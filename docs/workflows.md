# Example workflows

## Extract and re-basecall reads mapping to a particular genomic region

```sh
samtools view https://url/to/reads.bam chrX:147911919-147951125 | cut -f1  | sort -u > rid_list.txt
slow5curl get https://url/to/reads.blow5 --list rid_list.txt -o extracted.blow5
buttery-eel -i extracted.blow5  -g /path/to/ont-guppy/bin/ --config dna_r9.4.1_450bps_sup.cfg --device 'cuda:all' -o extracted_sup.fastq # see https://github.com/Psy-Fer/buttery-eel/ for butter-eel options
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

## Generate compressed TSV from BAM file to map genomic regions to reads

Below is a walkthrough on generating a TSV file from a BAM file. This is useful if you only intend on using the TSV to map genomic regions to read IDs, and want to save as much disk space as possible.

```sh
samtools view -h reads.bam -o reads.sam        # convert bam to sam
paftools.js sam2paf -p reads.sam > reads.paf   # convert sam to paf
cut -f1,6,8,9 reads.paf > reads.tsv            # generate tsv from paf
bgzip reads.tsv                                # compress
tabix tsv.gz -0 -s2 -b3 -e4                    # generate index
```

## Generate list of read IDs from region with compressed TSV

Note: The index of the compressed TSV must be present in its directory.

```sh
# get list of read IDs corresponding to a specific region
tabix reads.tsv.gz chrX:43,044,295-43,170,245 | cut -f 1 | sort -u > rid_list.txt

# get list of read IDs corresponding to a list of regions specified in a sorted bed file
tabix reads.tsv.gz -R regions_list.hg38.bed | cut -f 1 | sort -u > rid_list.txt

# get list of read IDs corresponding to a chromosome
tabix reads.tsv.gz chrX | cut -f 1 | sort -u > rid_list.txt
```

Note: Any list of regions used must be sorted, this can be done with:

```sh
bedtools sort -i unsorted_regions_list.hg38.bed > regions_list.hg38.bed
```
