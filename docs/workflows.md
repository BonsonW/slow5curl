# Example workflows

## Extract and re-basecall reads mapping to a particular genomic region

```sh
# extract region chrX:147911919-147951125
samtools view https://url/to/reads.bam chrX:147911919-147951125 | cut -f1  | sort -u > rid_list.txt

# OR

# extract regions from a BED file
samtools view https://url/to/reads.bam -L /path/to/regions.bed -M | cut -f1 | sort -u > rid_list.txt

# fetch reads corresponding to the extracted region
slow5curl get https://url/to/reads.blow5 --list rid_list.txt -o extracted.blow5

# basecall extracted BLOW5, see https://github.com/Psy-Fer/buttery-eel/ for butter-eel options
buttery-eel -i extracted.blow5  -g /path/to/ont-guppy/bin/ --config dna_r9.4.1_450bps_sup.cfg --device 'cuda:all' -o extracted_sup.fastq 
```

Note: If the read IDs in the BAM file are not the parent IDs (happens when read splitting is enabled during the initial basecalling step), you can grab the parent read IDs from the FASTQ file as below and use that as the input the to slow5curl get.
```sh
grep -F -f rid_list.txt reads.fastq | sed -n -e 's/.*parent\_read\_id=//p' | awk '{print $1}' | sort -u > parent_rid_list.txt
```
The above assumes that `parent_read_id` tag is present in all reads including those that are not split, which seem to be the case when doing live-basecalling. But Guppy offline version seem to only output that `parent_read_id` tag for split reads. In that case do this:
```sh
# for split reads, get the parent_read_id tag
grep -F -f rid_list.txt reads.fastq | sed -n -e 's/.*parent\_read\_id=//p' | awk '{print $1}' > tmp.txt

# for non split reads, get the normla read ID
grep -F -f rid_list.txt reads.fastq | grep -v "parent\_read\_id" | awk '{print $1}' | tr -d '@' >> tmp.txt

# remove duplicates
sort -u tmp.txt > parent_rid_list.txt
```
