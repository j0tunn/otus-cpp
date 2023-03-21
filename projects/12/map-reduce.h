#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <functional>
#include <utility>
#include <fstream>
#include <ctime>
#include <future>
#include <map>

namespace fs = std::filesystem;

template <typename T>
class MapReduce {
public:
    using ReduceAccType = T;
    using Mapper = std::function<std::string(const std::string&)>;
    using Reducer = std::function<ReduceAccType(const std::string&, const ReduceAccType&)>;
    using ReduceAccSerializer = std::function<std::string(const ReduceAccType&)>;

    MapReduce(const unsigned int mappersCount, const unsigned int reducersCount)
        : mappersCount_(mappersCount)
        , reducersCount_(reducersCount)
        , mapper_(nullptr)
        , reducer_(nullptr)
        , reduceAccSerializer_(nullptr)
        , workDir_("./killme" + std::to_string(std::time(0)) + "_" + std::to_string(clock()) + "/")
    {}

    MapReduce& setMapper(Mapper mapper) {
        mapper_ = mapper;
        return *this;
    }

    MapReduce& setReducer(Reducer reducer) {
        reducer_ = reducer;
        return *this;
    }

    MapReduce& setReduceAccSerializer(ReduceAccSerializer serializer) {
        reduceAccSerializer_ = serializer;
        return *this;
    }

    std::vector<std::string> run(const fs::path& inputFile, ReduceAccType initAcc) {
        fs::create_directory(workDir_);

        std::vector<fs::path> mapResults = map_(inputFile, getInputBorders_(inputFile));
        std::vector<fs::path> reduceInputFiles = shuffle_(mapResults);
        std::vector<fs::path> reduceResults = reduce_(reduceInputFiles, initAcc);

        return readFiles_(reduceResults);
    }

private:
    unsigned int mappersCount_;
    unsigned int reducersCount_;

    Mapper mapper_;
    Reducer reducer_;
    ReduceAccSerializer reduceAccSerializer_;
    fs::path workDir_;

    // - разбиваем файл на равные куски - это область, в рамках которой ищем переводы строк
    // - если последний чанк - кладем в него все до конца файла
    // - чанков может быть существенно меньше mappersCount_, если строк меньше
    std::vector<std::pair<unsigned, unsigned> > getInputBorders_(const fs::path& inputFile) {
        const uintmax_t inputSize = fs::file_size(inputFile);
        const unsigned rawChunkSize = inputSize / mappersCount_;
        std::ifstream inputStream(inputFile.c_str());

        std::vector<std::pair<unsigned, unsigned> > borders;
        borders.reserve(mappersCount_);

        for (uintmax_t offset = 0; offset < inputSize;) {
            const uintmax_t hint = borders.size() == mappersCount_ - 1
                ? inputSize - 1
                : std::min(offset + rawChunkSize, inputSize - 1);
            const uintmax_t p = findEolPos_(inputStream, hint, offset);
            if (!p) {
                break;
            }

            borders.push_back(std::make_pair(offset, p - offset));
            offset = p + 1;
        }

        return borders;
    }

    uintmax_t findEolPos_(std::ifstream& inStream, uintmax_t hint, uintmax_t lBound) {
        auto check = [&inStream](uintmax_t pos) {
            inStream.seekg(pos);
            return inStream.peek() == '\n';
        };

        if (check(hint)) {
            return hint;
        }

        for (unsigned int i = 1;; ++i) {
            if (lBound + i < hint && check(hint - i)) {
                return hint - i;
            }

            if (check(hint + i)) {
                return hint + i;
            }

            if (inStream.eof()) {
                break;
            }
        }

        return 0;
    }

    std::vector<fs::path> map_(const fs::path& inputFile, const std::vector<std::pair<unsigned, unsigned> >& borders) {
        std::vector<std::future<fs::path> > futures;
        futures.reserve(borders.size());

        for (unsigned int jobNumber = 0; jobNumber < borders.size(); ++jobNumber) {
            futures.push_back(std::async(std::launch::async, [this, inputFile, &borders, jobNumber]() {
                const auto& [offset, size] = borders[jobNumber];

                std::ifstream inStream(inputFile.c_str());
                inStream.seekg(offset);

                std::vector<std::string> mapResults;
                for (std::string line; inStream.tellg() < offset + size && getline(inStream, line);) {
                    mapResults.push_back(mapper_(line));
                }

                std::sort(mapResults.begin(), mapResults.end());

                const fs::path outFile = workDir_ / fs::path("mapper_out_" + std::to_string(jobNumber));
                std::ofstream outStream(outFile.c_str());
                for (const std::string& line : mapResults) {
                    outStream << line << std::endl;
                }

                return outFile;
            }));
        }

        std::vector<fs::path> outFiles;
        outFiles.reserve(futures.size());
        std::transform(futures.begin(), futures.end(), std::back_inserter(outFiles), [](auto& f) { return f.get(); });

        return outFiles;
    }

    // Для чтения в порядке сортировки складываем input-стримы в multimap. В качестве ключа используем последнюю прочитанную строку.
    // Output-стримы складываем в map. В качестве ключа используем последнюю записанную строку.
    // Алгоритм:
    // 1. Берем input-стрим с "наименьшей" строкой
    // 2. Берём output-стрим в следующем приоритете:
    //    - стрим c такой же строкой
    //    - новый стрим (если их меньше reducersCount_)
    //    - стрим, в который меньше всего писали
    // 3. Читаем из input-стрима одинаковые строки, и записываем их в output-стрим
    // 4. Если есть новая строка и она изменилась - кладем использованный input-стрим в multimap c input-стримами стримами уже с новой строкой
    // 5. Удаляем из multimap использованный input-stream с прочитанной строкой
    // Цикл завершится когда в multimap'е c input-стримами не останется стримов (они удаляются на шаге 5, и добавляются на шаге 4 в том случае, если есть новые строки)
    std::vector<fs::path> shuffle_(const std::vector<fs::path>& inputFiles) {
        std::multimap<const std::string, std::ifstream> inputs;
        for (const auto& inputFile : inputFiles) {
            std::ifstream inputStream(inputFile.c_str());
            std::string line;
            std::getline(inputStream, line);

            inputs.emplace(line, std::move(inputStream));
        }

        std::map<std::string, std::ofstream> outputs;
        std::vector<fs::path> outFiles;

        for(;;) {
            auto itInput = inputs.begin();
            if (itInput == inputs.end()) {
                break;
            }

            const std::string& line = itInput->first;
            std::ifstream& inStream = itInput->second;

            auto itOutput = outputs.find(line);

            if (itOutput == outputs.end() && outputs.size() < reducersCount_) {
                fs::path outFile = workDir_ / fs::path("reducer_in_" + std::to_string(outFiles.size()));
                outFiles.push_back(outFile);
                auto [it, success] = outputs.emplace(line, std::ofstream(outFile));

                if (success) {
                    itOutput = it;
                }
            }

            if (itOutput == outputs.end()) {
                itOutput = std::min_element(outputs.begin(), outputs.end(), [](auto& a, auto& b) {
                    return a.second.tellp() < b.second.tellp();
                });
            }

            std::ofstream& outStream = itOutput->second;

            outStream << line << std::endl;
            for (std::string nextLine; std::getline(inStream, nextLine);) {
                if (nextLine == line) {
                    outStream << nextLine << std::endl;
                } else {
                    inputs.emplace(nextLine, std::move(inStream));
                    break;
                }
            }

            inputs.erase(itInput);
        }

        return outFiles;
    }

    std::vector<fs::path> reduce_(const std::vector<fs::path>& inputFiles, ReduceAccType initAcc) {
        std::vector<std::future<fs::path> > futures;
        futures.reserve(inputFiles.size());

        for (unsigned int jobNumber = 0; jobNumber < inputFiles.size(); ++jobNumber) {
            futures.push_back(std::async(std::launch::async, [this, &inputFiles, jobNumber, initAcc]() {
                std::ifstream inStream(inputFiles[jobNumber].c_str());

                ReduceAccType acc = initAcc;
                for (std::string line; getline(inStream, line);) {
                    acc = reducer_(line, acc);
                }

                const fs::path outFile = workDir_ / fs::path("reducer_out_" + std::to_string(jobNumber));
                std::ofstream(outFile.c_str()) << reduceAccSerializer_(acc) << std::endl;

                return outFile;
            }));
        }

        std::vector<fs::path> outFiles;
        outFiles.reserve(futures.size());
        std::transform(futures.begin(), futures.end(), std::back_inserter(outFiles), [](auto& f) { return f.get(); });

        return outFiles;
    }

    std::vector<std::string> readFiles_(const std::vector<fs::path>& files) {
        std::vector<std::string> lines;

        for (const auto& file : files) {
            std::ifstream inStream(file.c_str());
            for (std::string line; getline(inStream, line);) {
                lines.push_back(line);
            }
        }

        return lines;
    }
};
