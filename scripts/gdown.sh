#!/bin/bash

INDEX_ID="10G9tXqLGWOaCIG1c6JHN6deT2dIme621"
BLOW5_ID="1X4p8BJkhEcqzHPu3f1OPN1NQmaXrDH0d"

OUT="outfile"

html=`curl -L --cookie-jar ./cookie "https://drive.google.com/uc?export=download&id=${BLOW5_ID}"`
# html_index=`curl --cookie-jar index_cookie "https://drive.google.com/uc?export=download&id=${INDEX_ID}"`

curl -L --cookie-jar ./cookie "https://drive.google.com/uc?export=download&id=${BLOW5_ID}"

# ./slow5curl head --cookie ./cookie "https://drive.google.com/uc?export=download&`echo ${html}|grep -Po '(confirm=[a-zA-Z0-9\-_]+)'`&id=${BLOW5_ID}"
# ./slow5curl reads --header-cookie header_cookie --index-cookie index_cookie --index "https://drive.google.com/uc?export=download&`echo ${html_index} | grep -Po '(confirm=[a-zA-Z0-9\-_]+)'`&id=${INDEX_ID}" "https://drive.google.com/uc?export=download&`echo ${html_header} | grep -Po '(confirm=[a-zA-Z0-9\-_]+)'`&id=${BLOW5_ID}" > reads.list
