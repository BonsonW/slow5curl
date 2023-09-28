# distutils: language = c
# cython: language_level=3
# cython: profile=True
cimport pys5curl

from slow5lib.python.pyslow5 import Open


cdef class Stream(Open):
    def __cinit__(self, pathname, index=None, DEBUG=0):
        # Set to default NULL type
        self.s5 = NULL
        self.rec = NULL
        self.read = NULL
        self.write = NULL
        self.twrite = NULL
        self.path = ""
        self.p = ""
        self.mode = ""
        self.m = ""
        self.rec_press = ""
        self.sig_press = ""
        # state for read/write. -1=null, 0=read, 1=write, 2=append
        self.state = -1
        self.num_read_groups = 0
        self.trec = NULL
        self.rid = NULL
        self.rids = NULL
        self.rids_len = 0
        self.index_state = False
        self.header_state = False
        self.header_add_attr_state = False
        self.close_state = False
        self.s5_aux_type = NULL
        self.s5_aux_enum = NULL
        self.aux_get_err = 0
        self.aux_get_len = 0
        self.head_len = 0
        self.aux_len = 0
        self.enum_len = 0
        self.shape_seq[0] = 0
        self.shape_get[0] = 0
        self.e0 = 0
        self.e1 = 0
        self.e2 = 0
        self.e3 = 0
        self.e4 = 0
        self.e5 = 0
        self.e6 = 0
        self.e7 = 0
        self.e8 = 0.0
        self.e9 = 0.0
        self.e10 = 0
        self.e11 = 0
        self.e12 = NULL
        self.e13 = NULL
        self.e14 = NULL
        self.e15 = NULL
        self.e16 = NULL
        self.e17 = NULL
        self.e18 = NULL
        self.e19 = NULL
        self.e20 = NULL
        self.e21 = NULL
        self.e22 = NULL
        self.channel_number = strdup("channel_number")
        self.median_before = strdup("median_before")
        self.read_number = strdup("read_number")
        self.start_mux = strdup("start_mux")
        self.start_time = strdup("start_time")
        self.end_reason = strdup("end_reason")
        self.tracked_scaling_shift = strdup("tracked_scaling_shift")
        self.tracked_scaling_scale = strdup("tracked_scaling_scale")
        self.predicted_scaling_shift = strdup("predicted_scaling_shift")
        self.predicted_scaling_scale = strdup("predicted_scaling_scale")
        self.num_reads_since_mux_change = strdup("num_reads_since_mux_change")
        self.time_since_mux_change = strdup("time_since_mux_change")
        self.num_minknow_events = strdup("num_minknow_events")
        self.end_reason_labels = NULL
        self.end_reason_labels_len = 0
        self.channel_number_val = NULL
        self.median_before_val = 0.0
        self.read_number_val = 0
        self.start_mux_val = 0
        self.start_time_val = 0
        self.end_reason_val = 0
        self.tracked_scaling_shift_val = 0.0
        self.tracked_scaling_scale_val = 0.0
        self.predicted_scaling_shift_val = 0.0
        self.predicted_scaling_scale_val = 0.0
        self.num_reads_since_mux_change_val = 0
        self.time_since_mux_change_val = 0.0
        self.num_minknow_events_val = 0
        self.channel_number_val_array = NULL
        self.median_before_val_array = NULL
        self.read_number_val_array = NULL
        self.start_mux_val_array = NULL
        self.start_time_val_array = NULL
        self.end_reason_val_array = NULL
        self.tracked_scaling_shift_val_array = NULL
        self.tracked_scaling_scale_val_array = NULL
        self.predicted_scaling_shift_val_array = NULL
        self.predicted_scaling_scale_val_array = NULL
        self.num_reads_since_mux_change_val_array = NULL
        self.time_since_mux_change_val_array = NULL
        self.num_minknow_events_val_array = NULL


        self.total_time_slow5_get_next = 0.0
        self.total_time_yield_reads = 0.0
        self.total_single_write_time = 0.0
        self.total_multi_write_signal_time = 0.0
        self.total_multi_write_time = 0.0

        # sets up logging level/verbosity
        self.V = DEBUG
        self.logger = logging.getLogger("pyslow5curl")
        loghandler = logging.StreamHandler()
        if self.V == 1:
            loghandler.setLevel(logging.DEBUG)
        else:
            loghandler.setLevel(logging.WARNING)
        formatter = logging.Formatter('%(asctime)s - %(name)s - [%(levelname)s]: %(message)s', datefmt='%d-%b-%y %H:%M:%S')
        loghandler.setFormatter(formatter)
        self.logger.addHandler(loghandler)
        if self.V == 1:
            self.logger.setLevel(logging.DEBUG)
        else:
            self.logger.setLevel(logging.WARNING)

        # logging.basicConfig(format='%(asctime)s - %(name)s - [%(levelname)s]: %(message)s',
        #                     datefmt='%d-%b-%y %H:%M:%S', level=lev)

        # This should match slow5_defs.h error codes
        self.error_codes = {-1: ["SLOW5_ERR_EOF", "End Of File reached"],
                            -2: ["SLOW5_ERR_ARG", "bad argument (NULL)"],
                            -3: ["SLOW5_ERR_TRUNC", "file truncated"],
                            -4: ["SLOW5_ERR_RECPARSE", "record parsing error"],
                            -5: ["SLOW5_ERR_IO", "other file I/O error"],
                            -6: ["SLOW5_ERR_NOIDX", "index not loaded"],
                            -7: ["SLOW5_ERR_NOTFOUND", "read id not found"],
                            -8: ["SLOW5_ERR_OTH", "other error (big endian, internal error, etc.)"],
                            -9: ["SLOW5_ERR_UNK", "file format unknown"],
                            -10: ["SLOW5_ERR_MEM", "memory (re)allocation error"],
                            -11: ["SLOW5_ERR_NOAUX", "no auxiliary map"],
                            -12: ["SLOW5_ERR_NOFLD", "field not found"],
                            -13: ["SLOW5_ERR_PRESS", "(de)compression failure"],
                            -14: ["SLOW5_ERR_MAGIC", "magic number invalid"],
                            -15: ["SLOW5_ERR_VERSION", "version incompatible"],
                            -16: ["SLOW5_ERR_HDRPARSE", "header parsing error"],
                            -17: ["SLOW5_ERR_TYPE", "error relating to slow5 type"]}

        # slow5curl codes
        #define S5CURL_ERR_OK       (0)     // Successfully completed
        #define S5CURL_ERR_OTH      (-1)    // Other error
        #define S5CURL_ERR_SLOW5    (-2)    // Error processing s/blow5 data 
        #define S5CURL_ERR_FETCH    (-3)    // Fetching data failed
        #define S5CURL_ERR_CURL     (-4)    // Invalid CURL connection
        self.slow5curl_error_codes = {-1: ["S5CURL_ERR_OTH", "Other error"],
                                      -2: ["S5CURL_ERR_SLOW5", "Error processing s/blow5 data"],
                                      -3: ["S5CURL_ERR_FETCH", "Fetching data failed"],
                                      -4: ["S5CURL_ERR_CURL", "Invalid CURL connection"]}

        # slow5_press_method from slow5_press.h
        # enum slow5_press_method {
        #     SLOW5_COMPRESS_NONE,
        #     SLOW5_COMPRESS_ZLIB,
        #     SLOW5_COMPRESS_SVB_ZD, /* streamvbyte zigzag delta */
        #     SLOW5_COMPRESS_ZSTD,
        # };
        # self.slow5_press_method = {"none": 0,
        #                            "zlib": 1,
        #                            "svb_zd": 2,
        #                            "svb-zd": 2,
        #                            "zstd": 3}

        p = str.encode(pathname)
        self.path = pathname
        self.p = strdup(p)

        idx = str.encode(index)
        self.index = index
        self.idx = strdup(idx)
        # m = str.encode(mode)
        # self.mode = mode
        # self.m = strdup(m)
        # self.logger.debug("FILE: {}, mode: {}".format(self.path, self.mode))
        # self.logger.debug("FILE: {}, mode: {}".format(self.p, self.m))
        # Set state based on mode for file opening
        # state for read/write. -1=null, 0=read, 1=write, 2=append

        # read only state for slow5curl
        self.state = 0

        # if mode == "r":
        #     self.state = 0
        # elif mode == "w":
        #     self.state = 1
        #     self.rec_press = rec_press
        #     self.sig_press = sig_press
        # elif mode == "a":
        #     self.state = 2
        # else:
        #     self.state = -1
        # opens file and creates slow5 object for reading
        # if self.state == 0:
        self.s5 = s5curl_open(self.p)
        self.logger.debug("Number of read_groups: {}".format(self.s5.header.num_read_groups))
        self.num_read_groups = self.s5.header.num_read_groups
        if index is not None:
             ret = s5curl_idx_load_with(self.s5, self.idx)
             if ret != 0:
                self.logger.error("s5curl_idx_load_with return not 0: {}: {}".format(ret, self.slow5curl_error_codes[ret]))
                raise RuntimeError("Unable to load index")
        else:
             ret = s5curl_idx_load(self.s5)
             if ret != 0:
                self.logger.error("s5curl_idx_load return not 0: {}: {}".format(ret, self.slow5curl_error_codes[ret]))
                raise RuntimeError("Unable to load index")

        # initialise CURL
        self.S5CURLCON = s5curl_conn_init()




        # elif self.state == 1:
        #     self.s5 = pyslow5.slow5_open(self.p, self.m)
        #     if self.s5 is NULL:
        #         self.logger.error("File '{}' could not be opened for writing.".format(self.path))
        #     if "blow5" in self.path.split(".")[-1]:
        #         if self.rec_press in self.slow5_press_method.keys() and self.sig_press in self.slow5_press_method.keys():
        #             ret = pyslow5.slow5_set_press(self.s5, self.slow5_press_method[self.rec_press], self.slow5_press_method[self.sig_press])
        #             if ret != 0:
        #                 self.logger.error("slow5_set_press return not 0: {}".format(ret))
        #                 raise RuntimeError("Unable to set compression")
        #         else:
        #             self.logger.error("Compression type rec_press: {}, sig_press: {} could not be found.".format(self.rec_press, self.sig_press))
        #             self.logger.error("Please use only the following: {}".format(",".join(press for press in self.slow5_press_method.keys())))
        #             raise KeyError
        #     else:
        #         self.logger.debug("Not writing blow5, skipping compression steps")
        # elif self.state == 2:
        #     self.s5 = pyslow5.slow5_open(self.p, self.m)
        #     if self.s5 is NULL:
        #         self.logger.error("File '{}' could not be opened for writing - appending.".format(self.path))
        # else:
        #     self.logger.error("File '{}' unknown open method: {}".format(self.path, self.mode))
        # check object was actually created.
        if self.s5 is NULL:
            raise MemoryError()
    
    def __init__(self, pathname, index=None, DEBUG=0):
        super().__init__()
    
    def __dealloc__(self):
        if self.p is not NULL:
            free(self.p)
        if self.idx is not NULL:
            free(self.idx)
        if self.rec is not NULL:
            slow5_rec_free(self.rec)
        if self.read is not NULL:
            slow5_rec_free(self.read)
        if not self.close_state:
            if self.s5 is not NULL:
                s5curl_idx_unload(self.s5)
                s5curl_close(self.s5)
                self.close_state = True
                self.logger.debug("{} closed".format(self.path))

        free(self.channel_number)
        free(self.median_before)
        free(self.read_number)
        free(self.start_mux)
        free(self.start_time)
        free(self.end_reason)
        free(self.tracked_scaling_shift)
        free(self.tracked_scaling_scale)
        free(self.predicted_scaling_shift)
        free(self.predicted_scaling_scale)
        free(self.num_reads_since_mux_change)
        free(self.time_since_mux_change)
        free(self.num_minknow_events)
        
        if self.end_reason_labels is not NULL:
            for i in range(self.end_reason_labels_len):
                free(self.end_reason_labels[i])
            free(self.end_reason_labels)

        self.logger.debug("pathname: {}".format(self.path))
        self.logger.debug("total_time_slow5_get_next: {} seconds".format(self.total_time_slow5_get_next))
        self.logger.debug("total_time_yield_reads: {} seconds".format(self.total_time_yield_reads))
        self.logger.debug("total_single_write_time: {} seconds".format(self.total_single_write_time))
        self.logger.debug("total_multi_write_signal_time: {} seconds".format(self.total_multi_write_signal_time))
        self.logger.debug("total_multi_write_time: {} seconds".format(self.total_multi_write_time))
    

    def _get_read(self, read_id, pA, aux):
        '''
        read_id = readID of individual read as a string, get's converted to b'str'
        pA = Bool for converting signal to picoamps
        aux = str 'name'/'all' or list of names of auxiliary fields added to return dictionary
        returns dic = dictionary of main fields for read_id, with any aux fields added
        '''
        # if not self.index_state:
        #     # self.logger.debug("FILE: {}, mode: {}".format(self.path, self.mode))
        #     # self.logger.debug("FILE: {}, mode: {}".format(self.path, self.m))
        #     self.logger.debug("loading index...")
        #     ret = s5curl_idx_load(self.s5)
        #     if ret != 0:
        #         self.logger.warning("slow5_idx_load return not 0: {}: {}".format(ret, self.error_codes[ret]))
        #     else:
        #         self.index_state = True
        dic = {}
        aux_dic = {}
        ID = str.encode(read_id)
        # rec = NULL
        # int s5curl_get(const char *read_id, slow5_rec_t **record, CURL *curl, s5curl_t *s5c);
        # ret = slow5_get(ID, &self.rec, self.s5)
        ret = s5curl_get(ID, &self.rec, self.S5CURLCON, self.s5)
        if ret != 0:
            self.logger.debug("get_read return not 0: {}".format(ret))
            return None

        # check for aux fields
        if aux is not None:
            if not self.aux_names or not self.aux_types:
                self.aux_names = self.get_aux_names()
                self.aux_types = self.get_aux_types()
            if type(aux) is str:
                # special type 'all'
                if aux == "all":
                    aux_dic = self._get_read_aux(self.aux_names, self.aux_types)
                else:
                    found_single_aux = False
                    for n, t in zip(self.aux_names, self.aux_types):
                        if n == aux:
                            found_single_aux = True
                            aux_dic = self._get_read_aux([n], [t])
                            break
                    if not found_single_aux:
                        self.logger.warning("get_read unknown aux name: {}".format(aux))
                        aux_dic.update({aux: None})
            elif type(aux) is list:
                n_list = []
                t_list = []
                for n, t in zip(self.aux_names, self.aux_types):
                    if n in aux:
                        n_list.append(n)
                        t_list.append(t)

                aux_dic = self._get_read_aux(n_list, t_list)
                # Check for items given that did not exist
                n_set = set(n_list)
                aux_set = set(aux)
                if len(aux_set.difference(n_set)) > 0:
                    for i in aux_set.difference(n_set):
                        self.logger.warning("get_read unknown aux name: {}".format(i))
                        aux_dic.update({i: None})

            else:
                self.logger.debug("get_read aux type unknown, accepts str or list: {}".format(aux))

        # get read data
        if type(self.rec.read_id) is bytes:
            dic['read_id'] = self.rec.read_id.decode()
        else:
            dic['read_id'] = self.rec.read_id
        dic['read_group'] = self.rec.read_group
        dic['digitisation'] = self.rec.digitisation
        dic['offset'] = self.rec.offset
        dic['range'] = self.rec.range
        dic['sampling_rate'] = self.rec.sampling_rate
        dic['len_raw_signal'] = self.rec.len_raw_signal
        # This could be handled by numpy.fromiter() or similar
        # Probably MUCH faster
        # https://stackoverflow.com/questions/7543675/how-to-convert-pointer-to-c-array-to-python-array
        # https://groups.google.com/g/cython-users/c/KnjF7ViaHUM
        # dic['signal'] = [self.rec.raw_signal[i] for i in range(self.rec.len_raw_signal)]
        # cdef np.npy_intp shape_get[1]
        self.shape_get[0] = <np.npy_intp> self.rec.len_raw_signal
        signal = copy.deepcopy(np.PyArray_SimpleNewFromData(1, self.shape_get,
                    np.NPY_INT16, <void *> self.rec.raw_signal))
        np.PyArray_UpdateFlags(signal, signal.flags.num | np.NPY_OWNDATA)
        dic['signal'] = signal

        # if pA=True, convert signal to pA
        if pA:
            dic['signal'] = self._convert_to_pA(dic)
        # if aux data, add to main dic
        if aux_dic:
            dic.update(aux_dic)
        return dic