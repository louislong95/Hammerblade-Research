This example runs Sparse Matrix * Dense Matrix on:

## For 16x16 Sparse Matrix - Dense Matrix Multiplication:
- `v0`: a single tile (1x1 grid of 1x1 tile group)
- `v1`: a square tile group (1x1 grid of 2x2 tile group)
- `v2`: a square tile group in a 4x1 grid (4x1 grid of 2x2 tile group)
- `v3`: a square tile group(1x1 grid of 4x4 tile group)

For all versions, the data is copied to the scratchpad before the result is calculated.
Additionally, the datatype used was float.

| Version | # Instructions | # Tile Cycles | Abs. Group Cycles | IPC     | Stalled Cycles | stall_md cycles | stall_depend | misses | hit rate |
|---------|----------------|----------|-------------|---------|----------------|-----------------|--------------|--------|----------|
| v0      | 8662           | 14956    | 14956       | 0.5792  | 6004           | 1373            | 4629         | 145    | 98.33%   |
| v1      | 11901          | 21656    | 5558        | 0.5495  | 9363           | 1940            | 7307         | 197    | 98.34%   |
| v2      | 24833          | 48003    | 12893       | 0.5173  | 22350          | 4208            | 17866        | 413    | 98.34%   |
| v3      | 24835          | 47864    | 3859        | 0.5189  | 22213          | 4208            | 17004        | 413    | 98.34%   |



## For 10x500 Lenet Layer Data:
- `v0`: 1x1 grid of 1x1 tile group with scratchpad
- `v1`: 1x1 grid of 1x1 tile group without scratchpad
- `v2`: 1x1 grid of 1x2 tile group with scratchpad
- `v3`: 1x1 grid of 1x2 tile group without scratchpad
- `v8`: 1x1 grid of 2x2 tile group with scratchpad
- `v9`: 1x1 grid of 2x2 tile group without scratchpad
- `v12`: 1x1 grid of 2x4 tile group with scratchpad
- `v16`: 1x1 grid of 4x4 tile group with scratchpad

W/ Scratchpad: Parallelized by the columns of Dense 500x16
W/O Scratchpad: Iterate over rows of sparse matrix


| Version | # Instructions | # Tile Cycles | Abs. Group Cycles | IPC     | Stalled Cycles | stall_md cycles | stall_depend | misses | hit rate |
|---------|----------------|---------------|-------------|---------|----------------|-----------------|--------------|--------|----------|
| v0      | 274181         | 851711        | 851711      | 0.3219  | 576920         | 184             | 575920       | 305    | 99.89%   |
| v1      | 214560         | 937963        | 937963      | 0.2288  | 720277         | 57571           | 662655       | 1563   | 99.27%   |
| v2      | 274234         | 852743        | 426718      | 0.3216  | 577897         | 367             | 576713       | 306    | 99.89%   |
| v3      | 214614         | 934780        | 487704      | 0.2296  | 717037         | 57717           | 659268       | 1565   | 99.27%   |
| v8      | 274341         | 827948        | 213777      | 0.3314  | 552991         | 732             | 551183       | 309    | 99.89%   |
| v9      | 214722         | 917886        | 376765      | 0.2339  | 700028         | 58008           | 641965       | 1569   | 99.27%   |
| v12     | 274557         | 831051        | 107688      | 0.3304  | 555866         | 1456            | 553229       | 317    | 99.88%   |
| v16     | 274993         | 794550        | 54681       | 0.3461  | 518901         | 2896            | 513717       | 333    | 99.88%   |
