/**
 * @file ci_flash_data_info.h
 * @brief flash data struct
 * @version 
 * @date 
 * 
 * @copyright Copyright (c) 2019 Chipintelli Technology Co., Ltd.
 * 
 */

#ifndef _CI_FALSH_DATA_INFO_H_
#define _CI_FALSH_DATA_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif


#define FILECONFIG_SPIFLASH_START_ADDR      (0x2000)

#define COMMAND_INFO_FILE_ID            60000
#define VOICE_PRINT_DNN_ID              60001



#pragma pack(1)
typedef struct
{
    uint32_t manu_facturer_id;    //32Bit
    uint32_t product_id[2];      //64Bit (MAC Address)
//    
    uint32_t hard_ware_name[16];        //String
    uint32_t hard_ware_version;         //Vm.n.x.y
    uint32_t soft_ware_name[16];        //String,Exporting to Packet file name
    uint32_t soft_ware_version;         //Vm.n.x.y
//    
    uint32_t bootloader_version; //Vm.n.x.y
//    
    uint8_t reserve[14];
//    
    uint32_t user_code1_version;
    uint32_t user_code1_offset;
    uint32_t user_code1_size;
    uint32_t user_code1_crc;
    uint8_t user_code1_status;
//    
    uint32_t user_code2_version;
    uint32_t user_code2_offset;
    uint32_t user_code2_size;
    uint32_t user_code2_crc;
    uint8_t user_code2_status;
//    
    uint32_t asr_cmd_model_version;
    uint32_t asr_cmd_model_offset;
    uint32_t asr_cmd_model_size;
    uint32_t asr_cmd_model_crc;
    uint8_t asr_cmd_model_status;
//    
    uint32_t dnn_model_version;
    uint32_t dnn_model_offset;
    uint32_t dnn_model_size;
    uint32_t dnn_model_crc;
    uint8_t dnn_model_status;
//    
    uint32_t voice_version;
    uint32_t voice_offset;
    uint32_t voice_size;
    uint32_t voice_crc;
    uint8_t voice_status;
//
    uint32_t user_file_version;
    uint32_t user_file_offset;
    uint32_t user_file_size;
    uint32_t user_file_crc;
    uint8_t user_file_status;
//
    uint32_t nv_data_offset;
    uint32_t nv_data_size;
}partition_table_t; 
#pragma pack()


#pragma pack(1)
typedef struct
{
    uint16_t file_id;
    uint32_t file_addr;
    uint32_t file_size;
}file_header_t;

typedef struct
{
    uint16_t file_number;
    file_header_t file_header[1];
}file_table_t;           //此结构当作变长数组用

typedef struct
{
    uint16_t group_id;
    uint32_t group_addr;
}group_header_t;

typedef struct
{
    uint16_t group_number;
    group_header_t group_header[1];
}group_table_t;           //此结构当作变长数组用

typedef struct
{
    uint32_t hard_ware_version;
    uint32_t soft_ware_version;
}product_version_t;

#pragma pack()

extern uint32_t ci_flash_data_info_init(uint8_t default_model_group_id);

extern uint32_t get_current_model_addr(uint32_t *p_dnn_addr, uint32_t *p_dnn_size, uint32_t *p_asr_addr, uint32_t *p_addr_size);

extern uint32_t get_group_addr(uint32_t partition_addr, uint16_t group_id);

extern uint32_t get_file_addr(uint32_t group_addr, uint16_t file_id, uint32_t *p_file_addr, uint32_t *p_file_size);

extern uint32_t get_dnn_addr_by_id(uint16_t dnn_file_id, uint32_t *p_dnn_addr, uint32_t *p_dnn_size);

extern file_table_t * get_file_table(uint32_t group_addr);

extern void release_file_table(file_table_t * p_file_table);

extern int32_t get_fw_version(product_version_t *product_version);

extern uint32_t get_userfile_addr(uint16_t file_id, uint32_t *p_file_addr, uint32_t *p_file_size);



extern uint32_t cached_flash_reader_init(uint32_t start_addr_in_flash);

extern uint32_t cached_flash_reader_read(int32_t read_offset, uint8_t *read_buffer, uint32_t read_length);

extern uint32_t cached_flash_reader_destroy();



extern uint32_t cached_flash_reader_init(uint32_t start_addr_in_flash);

extern uint32_t cached_flash_reader_read(int32_t read_offset, uint8_t *read_buffer, uint32_t read_length);

extern uint32_t cached_flash_reader_destroy();

extern uint32_t get_asr_addr_by_id(int asr_id, uint32_t *p_asr_addr, uint32_t *p_asr_size);


#ifdef __cplusplus
}
#endif

#endif

