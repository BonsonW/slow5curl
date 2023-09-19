# Bash One-liners with slow5curl


## Extracting information for eye-balling and inspecting

**Note that these commands are not efficient to be run routinely on gigantic datasets, instead are only for quickly eyeballing and inspecting relatively smaller datasets.**

```
# print the raw-signal (column 8) for the read-id r1:
slow5curl get --to slow5 <BLOW5URL> "r1" | grep -v '^[#@]' | awk '{print $8}'

# extract the signal samples 100-200 (1-indexed) for the read-id r1:
slow5curl get --to slow5 <BLOW5URL> "r1" | grep -v '^[#@]' | awk '{print $8}' | cut -d, -f 100-200
```
