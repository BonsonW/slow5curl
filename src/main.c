/* @file main.c
**
******************************************************************************/

#include <stdio.h>
#include <slow5/slow5.h>

#define TO_PICOAMPS(RAW_VAL,DIGITISATION,OFFSET,RANGE) (((RAW_VAL)+(OFFSET))*((RANGE)/(DIGITISATION)))

int main(int argc, char* argv[]){

    if(argc < 2){
        fprintf(stderr,"Usage: %s <file>\n", argv[0]);
        return 1;
    }

    slow5_file_t *sp = slow5_open(argv[1],"r");
    if(sp==NULL){
       fprintf(stderr,"Error in opening file\n");
       return 1;
    }
    slow5_rec_t *rec = NULL;
    int ret=0;

    while((ret = slow5_get_next(&rec,sp)) >= 0){
        printf("%s\t",rec->read_id);
        uint64_t len_raw_signal = rec->len_raw_signal;
        double sum = 0;
        for(uint64_t i=0;i<len_raw_signal;i++){
            double pA = TO_PICOAMPS(rec->raw_signal[i],rec->digitisation,rec->offset,rec->range);
            sum += pA;
        }
        double mean = sum/len_raw_signal;
        printf("%f\n",mean);
    }

    if(ret != SLOW5_ERR_EOF){  //check if proper end of file has been reached
        fprintf(stderr,"Error in slow5_get_next. Error code %d\n",ret);
        return 1;
    }

    slow5_rec_free(rec);

    slow5_close(sp);

    return 0;
}
