#!/bin/bash

set -euo pipefail

# get appropriate URLs from:
# https://thredds.nci.org.au/thredds/catalog/catalogs/de95/catalog.html

BLOW5="https://thredds.nci.org.au/thredds/fileServer/de95/1KGP/raw/blow5/HG00097_R10/HG00097-M1252-ONT-WGS-LSK114-01.blow5"
BAM="https://thredds.nci.org.au/thredds/fileServer/de95/1KGP/analyses/aligned_bam_hg38/HG00097_R10/HG00097-M1252-ONT-WGS-LSK114-01.hg38.bam"
REGION="chr22"

EXTRACTED_BLOW5="extracted.blow5"

SAMTOOLS="samtools"
SLOW5CURL="./slow5curl"

die() {
	msg="$1"
    echo "Error: $msg" >&2
    exit 1
}

$SAMTOOLS --version &> /dev/null || { echo -e $RED"samtools not found! Either put samtools under path or set SAMTOOLS variable, e.g.,export SAMTOOLS=/path/to/samtools"$NORMAL; exit 1;}
$SLOW5CURL --version &> /dev/null || { echo -e $RED"slow5curl not found! Either put slow5curl under path or set SLOW5CURL variable, e.g.,export SLOW5CURL=/path/to/slow5curl"$NORMAL; exit 1;}

READ_IDS="rids.txt"

echo "Extracting read IDs from BAM for region $REGION..."
$SAMTOOLS view $BAM $REGION | cut -f1  | sort -u > $READ_IDS || die "Failed to extract read IDs from BAM"

echo "Extracting reads from BLOW5 for region $REGION..."
$SLOW5CURL get $BLOW5 --list $READ_IDS -t8 --skip -o $EXTRACTED_BLOW5 || die "Failed to extract reads from BLOW5"

# rm $READ_IDS

echo "successfully extracted reads from $REGION and saved to $EXTRACTED_BLOW5"
