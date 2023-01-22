```
$ ./bayan --help
Usage: bayan [options] [dir1 dir2 ...]
Allowed options:
  --help                        produce help message
  --dir arg (=[cwd])            include dirs
  --exclude arg                 directory names to exclude
  --file arg                    file masks allowed to compare (case
                                insensitive)
  --scan-level arg (=0)         scan level
  --min-size arg (=1)           min file size (in bytes)
  -S [ --chunk-size ] arg (=64) chunk size to read per iteration
  -H [ --hash ] arg (=md5)      hash type (md5 | crc32)
```
