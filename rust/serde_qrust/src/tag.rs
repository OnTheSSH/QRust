#![allow(dead_code)]

pub const VOID:         u8  = 0;
pub const BOOL:         u8  = 1;
pub const I8:           u8  = 2;
pub const I16:          u8  = 3;
pub const I32:          u8  = 4;
pub const I64:          u8  = 5;
pub const U8:           u8  = 6;
pub const U16:          u8  = 7;
pub const U32:          u8  = 8;
pub const U64:          u8  = 9;
pub const F32:          u8  = 10;   //0x0A
pub const F64:          u8  = 11;   //0x0B
pub const STR:          u8  = 20;   //0x14  String <--> QString
pub const STR_PTR:      u8  = 21;   //0x15  &str <--> char*  指针类型
pub const BYTES:        u8  = 22;   //0x16  Vec<u8> <--> QByteArray
pub const BYTES_PTR:    u8  = 23;   //0x17  &[u8] <--> QByteArray*  指针类型
pub const STRUCT:       u8  = 24;   //0x18

pub const LIST_BOOL:    u8  = 50;   //0x32
pub const LIST_I8:      u8  = 51;   //0x33
pub const LIST_I16:     u8  = 52;   //0x34
pub const LIST_I32:     u8  = 53;   //0x35
pub const LIST_I64:     u8  = 54;   //0x36
pub const LIST_U8:      u8  = 55;   //0x37
pub const LIST_U16:     u8  = 56;   //0x38
pub const LIST_U32:     u8  = 57;   //0x39
pub const LIST_U64:     u8  = 58;   //0x3A
pub const LIST_F32:     u8  = 59;   //0x3B
pub const LIST_F64:     u8  = 60;   //0x3C
pub const LIST_STR:     u8  = 61;   //0x3D
pub const LIST_BYTES:   u8  = 62;   //0x3E
pub const LIST_STRUCT:  u8  = 63;   //0x3F

pub const HASHI_BOOL:   u8  = 100;  //0x64
pub const HASHI_I8:     u8  = 101;  //0x65
pub const HASHI_I16:    u8  = 102;  //0x66
pub const HASHI_I32:    u8  = 103;  //0x67
pub const HASHI_I64:    u8  = 104;  //0x68
pub const HASHI_U8:     u8  = 105;  //0x69
pub const HASHI_U16:    u8  = 106;  //0x6A
pub const HASHI_U32:    u8  = 107;  //0x6B
pub const HASHI_U64:    u8  = 108;  //0x6C
pub const HASHI_F32:    u8  = 109;  //0x6D
pub const HASHI_F64:    u8  = 110;  //0x6E
pub const HASHI_STR:    u8  = 111;  //0x6F
pub const HASHI_BYTES:  u8  = 112;  //0x70
pub const HASHI_STRUCT: u8  = 113;  //0x71

pub const HASHS_BOOL:   u8  = 150;  //0x96
pub const HASHS_I8:     u8  = 151;  //0x97
pub const HASHS_I16:    u8  = 152;  //0x98
pub const HASHS_I32:    u8  = 153;  //0x99
pub const HASHS_I64:    u8  = 154;  //0x9A
pub const HASHS_U8:     u8  = 155;  //0x9B
pub const HASHS_U16:    u8  = 156;  //0x9C
pub const HASHS_U32:    u8  = 157;  //0x9D
pub const HASHS_U64:    u8  = 158;  //0x9E
pub const HASHS_F32:    u8  = 159;  //0x9F
pub const HASHS_F64:    u8  = 160;  //0xA0
pub const HASHS_STR:    u8  = 161;  //0xA1
pub const HASHS_BYTES:  u8  = 162;  //0xA2
pub const HASHS_STRUCT: u8  = 163;  //0xA3

pub const FUN_NAME:     u8  = 200;  //0xC8  函数名tag
pub const FUN_ARGS:     u8  = 201;  //0xC9  参数tag
pub const RET_VOID:     u8  = 202;  //0xCA  函数返回void
pub const RET_ARGS:     u8  = 203;  //0xCA  函数返回多个返回值（Rust是元组）

pub const FUN_NAME_ONT_EXIST:       u8  =   0xFF;   //请求函数名不存在
pub const FUN_ARGS_ERROR:           u8  =   0xFE;   //请求函数参数错误
pub const INVOKE_FAILURE:           u8  =   0xFD;   //invoke函数失败
pub const SERIALIZATION_FAILURE:    u8  =   0xFC;   //序列化失败
pub const DESERIALIZATION_FAILURE:  u8  =   0xFB;   //反序列化失败