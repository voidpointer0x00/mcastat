# mcastat

A simple Linux[^1] command-line utility that just _(at this point)_ shows 
.mca [region][RegionUrl] location header information.

It was actually written as a part of research, I wanted to be able to find
an average world size depending on the amount on chunks, that is covered
in [Advanced](#advanced) section.

The program basically reads _any_ 4KiB file and processes it like a `.mca`
region file, because the Anvil format does not define any special format
prefix.

## Building and Installing

```bash
# Generated Makefile for your system
cmake CMakeLists.txt
# builds the project
make
# installs the program into your /usr/local/bin folder, so
# you always have an access to it and can remove the build folder
sudo cp mcastat /usr/local/bin/
# remove build files
make clean
```

## Usage

The most simple command would be
```bash
mcastat world_nether/DIM-1/region/r.0.0.mca
```
With the following output _(data collected using seed `3625364`)_
```bash
File: world_nether/DIM-1/region/r.0.0.mca
Chunks: 1024
Sectors: 1376
Correlation: 1.343750 # turned out to be rather useless if you ask me
```

### Advanced

Getting total number of chunks in regions. I had `285 700` chunks in my
nether world.
```bash
find region/ -name "*.mca" | xargs -l1 mcastat -qc | awk '{s+=$1} END {print s}'
```

Getting average correlation using `gcalccmd` cli calculator. This gave me
`1.404533951`. So on average 285 700 chunks will have 1.404533951 sectors
using seed `3625364` _(note that no players ever visited the nether)_.
Knowing that each sector weights 4KiB we can predict that the total size
of all compressed sectors would be
`(285,700 * 1.404533951) * 4096 = 1,568MiB` _(which is not the world size yet)_

```bash
# getting correlation for all .mca files
find region/ -name "*.mca" | xargs -l1 mcastat -qp > corr.tmp
# sum up all correlations and divide by their total number
gcalccmd $(awk '{s+=$1} END {print s}' < corr.tmp)/$(wc -l < corr.tmp)
rm corr.tmp
```

But now let's find region size to chunks number correlation, and then it's
average. I got `6.159660494` bytes per chunk on average -> `1,759,815.0031358`
bytes in total, which really came close to the real size of `1,800,480` bytes.
```bash
for file in region/*.mca; do
  size=$(du $file | awk '{print $1}')
  chunks=$(mcastat -qc $file)
  gcalccmd $size/$chunks
done > size_corr.tmp
gcalccmd $(awk '{s+=$1} END {print s}' < size_corr.tmp)/$(wc -l < size_corr.tmp)
rm size_corr.tmp
```


[^1]: I'm not going to build it on Windows or any other platform, so I cannot
confirm that it works on any OS other than Linux. Even though the code does
not use any Linux exclusive API, the path to a file depends on the system
and, for example, using `~` with `mcastat` will transform into user's home
directory, but it won't on any other OS. _(As I'm not going to test and add
this feature)_. Also, I don't think it would have any purpose in Windows :)

[RegionUrl]: https://wiki.vg/Region_Files
