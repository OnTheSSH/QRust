use crate::error::{Error, Result};
use crate::tag;
use serde::ser::{self, Serialize};
use std::ffi::CString;
use log::{debug, error};

pub struct Serializer 
{
    output:             Vec<u8>,    //输出
    struct_name:        String,     //struct名称
    field_len:          u32,        //struct字段的数量
    body_indexs:        Vec<usize>, //记录集合体开始的位置
}

pub fn to_pack<T>(value: &T) -> Result<Vec<u8>>
where
    T: Serialize,
{
    let mut serializer = Serializer {
        output:             Vec::new(),
        struct_name:        String::new(),
        field_len:          0,
        body_indexs:        Vec::new(),
    };
    value.serialize(&mut serializer)?;
    Ok(serializer.output)
}

impl<'a> ser::Serializer for &'a mut Serializer 
{
    type Ok = ();
    type Error = Error;

    type SerializeSeq = Self;
    type SerializeTuple = Self;
    type SerializeTupleStruct = Self;
    type SerializeTupleVariant = Self;
    type SerializeMap = Self;
    type SerializeStruct = Self;
    type SerializeStructVariant = Self;

    fn serialize_bool(self, v: bool) -> Result<()> 
    {
        // println!("** serialize_bool()");
        self.output.push(tag::BOOL);
        if v { self.output.push(1u8); } else { self.output.push(0u8); }
        Ok(())
    }

    fn serialize_i8(self, v: i8) -> Result<()> 
    {
        // println!("** serialize_i8()");
        self.output.push(tag::I8);
        self.output.push(v as u8);
        Ok(())
    }

    fn serialize_i16(self, v: i16) -> Result<()> 
    {
        // println!("** serialize_i16()");
        self.output.push(tag::I16);
        let bytes: [u8; 2] = v.to_be_bytes();
        self.output.extend(bytes);
        Ok(())
    }

    fn serialize_i32(self, v: i32) -> Result<()> 
    {
        // println!("** serialize_i32()");
        self.output.push(tag::I32);
        let bytes: [u8; 4] = v.to_be_bytes();
        self.output.extend(bytes);
        Ok(())
    }

    fn serialize_i64(self, v: i64) -> Result<()> 
    {
        // println!("** serialize_i64()");
        self.output.push(tag::I64);
        let bytes: [u8; 8] = v.to_be_bytes();
        self.output.extend(bytes);
        Ok(())
    }

    fn serialize_u8(self, v: u8) -> Result<()> 
    {
        // println!("** serialize_u8()");
        self.output.push(tag::U8);
        self.output.push(v);
        Ok(())
    }

    fn serialize_u16(self, v: u16) -> Result<()> 
    {
        // println!("** serialize_u16()");
        self.output.push(tag::U16);
        let bytes: [u8; 2] = v.to_be_bytes();
        self.output.extend(bytes);
        Ok(())
    }

    fn serialize_u32(self, v: u32) -> Result<()> 
    {
        // println!("** serialize_u32()");
        self.output.push(tag::U32);
        let bytes: [u8; 4] = v.to_be_bytes();
        self.output.extend(bytes);
        Ok(())
    }

    fn serialize_u64(self, v: u64) -> Result<()> 
    {
        // println!("** serialize_u64()");
        self.output.push(tag::U64);
        let bytes: [u8; 8] = v.to_be_bytes();
        self.output.extend(bytes);
        Ok(())
    }

    fn serialize_f32(self, v: f32) -> Result<()> 
    {
        // println!("** serialize_f32()");
        self.output.push(tag::F32);
        let bytes: [u8; 4] = v.to_be_bytes();
        self.output.extend(bytes);
        Ok(())
    }

    fn serialize_f64(self, v: f64) -> Result<()> 
    {
        // println!("** serialize_f64()");
        self.output.push(tag::F64);
        let bytes: [u8; 8] = v.to_be_bytes();
        self.output.extend(bytes);
        Ok(())
    }

    fn serialize_char(self, _v: char) -> Result<()> 
    {
        unimplemented!()
    }

    fn serialize_str(self, v: &str) -> Result<()> 
    {
        // println!("** serialize_str()");
        self.output.push(tag::STR);
        let bytes = v.as_bytes().to_vec();
        let len_bytes: [u8; 4] = (bytes.len() as u32).to_be_bytes();
        self.output.extend(len_bytes);
        self.output.extend(bytes);
        Ok(())
    }

    fn serialize_bytes(self, v: &[u8]) -> Result<()> 
    {
        // println!("** serialize_bytes()");
        self.output.push(tag::BYTES);
        let len = v.len();
        let len_bytes: [u8; 4] = (len as u32).to_be_bytes();
        self.output.extend(len_bytes);
        self.output.extend(v);
        Ok(())
    }

    fn serialize_none(self) -> Result<()> 
    {
        unimplemented!()
    }

    fn serialize_some<T>(self, _value: &T) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        unimplemented!()
    }

    fn serialize_unit(self) -> Result<()> 
    {
        unimplemented!()
    }

    fn serialize_unit_struct(self, _name: &'static str) -> Result<()> 
    {
        unimplemented!()
    }

    fn serialize_unit_variant(self, _name: &'static str, _variant_index: u32, _variant: &'static str,) -> Result<()> 
    {
        unimplemented!()
    }

    fn serialize_newtype_struct<T>(self, _name: &'static str, _value: &T,) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        unimplemented!()
    }

    fn serialize_newtype_variant<T>(self, _name: &'static str, _variant_index: u32,
                                    _variant: &'static str, _value: &T,) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        unimplemented!()
    }

    // 现在我们开始序列化复合类型。
    //
    // 序列化开始、序列化值、序列化结束是三个单独的方法调用，
    // 此方法仅负责序列开始, 这个调用之后必须调用0个或多个`serialize_element`和`end`
    fn serialize_seq(self, _len: Option<usize>) -> Result<Self::SerializeSeq> 
    {
        // println!("** serialize_seq()");
        self.body_indexs.push(self.output.len());    //集合体开始的位置
        Ok(self)
    }

    fn serialize_tuple(self, _len: usize) -> Result<Self::SerializeTuple> 
    {
        self.body_indexs.push(self.output.len());    //集合体开始的位置
        Ok(self)
    }

    fn serialize_tuple_struct(self, _name: &'static str, _len: usize,) -> Result<Self::SerializeTupleStruct> 
    {
        unimplemented!()
    }

    fn serialize_tuple_variant(self, _name: &'static str, _variant_index: u32,
                                _variant: &'static str, _len: usize,) -> Result<Self::SerializeTupleVariant> 
    {
        unimplemented!()
    }

    // 开始序列化一个映射。这个调用之后必须有零次或多次调用' serialize_key '和' serialize_value '，然后调用' end '。
    fn serialize_map(self, _len: Option<usize>) -> Result<Self::SerializeMap> 
    {
        // println!("** serialize_map()");
        self.body_indexs.push(self.output.len());    //集合体开始的位置
        Ok(self)
    }

    //开始序列化一个结构体，这个调用之后必须有零次或多次对' serialize_field '的调用，然后再调用' end '。
    //'name'是结构体的名称，'len'是将被序列化的数据字段的数量。'len'不包括使用['SerializeStruct::skip_field']跳过的字段。
    fn serialize_struct(self, name: &'static str, len: usize,) -> Result<Self::SerializeStruct> 
    {
        // println!("** serialize_struct()");
        self.struct_name = String::from(name);  //struct名称
        self.field_len = len as u32;            //字段数量
        self.body_indexs.push(self.output.len());    //集合体开始的位置
        Ok(self)
    }

    fn serialize_struct_variant(self, _name: &'static str, _variant_index: u32,
        _variant: &'static str, _len: usize,) -> Result<Self::SerializeStructVariant> 
    {
        unimplemented!()
    }

}

// 接下来的 7 个 impl 处理复合类型（如序列和映射）的序列化。序列化此类类型是由 Serializer 方法开始的，
// 并跟随零个或多个序列化单个元素和一个结束复合类型的调用。

// 此 impl 是 SerializeSeq，因此这些方法是在 Serializer 上调用`serialize_seq`之后调用的。
impl<'a> ser::SerializeSeq for &'a mut Serializer 
{
    type Ok = ();
    type Error = Error;

    fn serialize_element<T>(&mut self, value: &T) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        value.serialize(&mut **self)
    }

    fn end(self) -> Result<()> 
    {
        // println!("SerializeSeq end(): {:?}", self.output);
        //split_off()函数将self.output从body_index处截断，截断的部分赋值给body
        let body = self.output.split_off(self.body_indexs.pop().unwrap());

        let mut head_buf = Vec::new();
        if body.is_empty()  //空集合
        {
            head_buf.push(tag::VOID);   //空集合不关心类型，使用VOID占位
        }
        else  //非空集合
        {
            let tag = match body[0]
            {
                tag::BOOL => tag::LIST_BOOL,
                tag::I8 => tag::LIST_I8,
                tag::I16 => tag::LIST_I16,
                tag::I32 => tag::LIST_I32,
                tag::I64 => tag::LIST_I64,
                tag::U8 => tag::LIST_U8,
                tag::U16 => tag::LIST_U16,
                tag::U32 => tag::LIST_U32,
                tag::U64 => tag::LIST_U64,
                tag::F32 => tag::LIST_F32,
                tag::F64 => tag::LIST_F64,
                tag::STR => tag::LIST_STR,
                tag::BYTES => tag::LIST_BYTES,
                tag::STRUCT => tag::LIST_STRUCT,
                _ => unimplemented!()
            };
            head_buf.push(tag);
        }

        let len = body.len();
        let len_bytes: [u8; 4] = (len as u32).to_be_bytes();
        head_buf.extend(len_bytes);
        
        self.output.extend(head_buf);
        self.output.extend(body);
        Ok(())
    }
}

impl<'a> ser::SerializeTuple for &'a mut Serializer {
    type Ok = ();
    type Error = Error;

    fn serialize_element<T>(&mut self, value: &T) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        value.serialize(&mut **self)
    }

    fn end(self) -> Result<()> 
    {
        //split_off()函数将self.output从body_index处截断，截断的部分赋值给body
        let body = self.output.split_off(self.body_indexs.pop().unwrap());

        let mut head_buf = Vec::new();
        head_buf.push(tag::FUN_ARGS);

        let len = body.len();
        let len_bytes: [u8; 4] = (len as u32).to_be_bytes();
        head_buf.extend(len_bytes);
        
        self.output.extend(head_buf);
        self.output.extend(body);
        Ok(())
    }
}

impl<'a> ser::SerializeTupleStruct for &'a mut Serializer {
    type Ok = ();
    type Error = Error;

    fn serialize_field<T>(&mut self, _value: &T) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        unimplemented!()
    }

    fn end(self) -> Result<()> 
    {
        unimplemented!()
    }
}

impl<'a> ser::SerializeTupleVariant for &'a mut Serializer {
    type Ok = ();
    type Error = Error;

    fn serialize_field<T>(&mut self, _value: &T) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        unimplemented!()
    }

    fn end(self) -> Result<()> 
    {
        unimplemented!()
    }
}

impl<'a> ser::SerializeMap for &'a mut Serializer 
{
    type Ok = ();
    type Error = Error;

    fn serialize_key<T>(&mut self, key: &T) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        key.serialize(&mut **self)
    }

    fn serialize_value<T>(&mut self, value: &T) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        value.serialize(&mut **self)
    }

    fn end(self) -> Result<()> 
    {
        let body = self.output.split_off(self.body_indexs.pop().unwrap());

        let mut head_buf = Vec::new();
        if body.is_empty()  //空集合
        {
            head_buf.push(tag::VOID);   //空集合不关心类型，使用VOID占位
        }
        else  //非空集合
        {
            //第一个元素的key和val的tag
            let key_is_int = body[0] ==tag::I32; //key是否是int
            let tag_val = 
                if key_is_int 
                {
                    body[5]
                }
                else    //key是字符串
                {
                    let mut bytes4 = [0u8; 4];
                    bytes4[0] = body[1];
                    bytes4[1] = body[2];
                    bytes4[2] = body[3];
                    bytes4[3] = body[4];
                    let key_str_len = u32::from_be_bytes(bytes4) as usize;
                    body[5 + key_str_len as usize]
                };          
            let tag = match tag_val
            {
                tag::BOOL => if key_is_int {tag::HASHI_BOOL}else{tag::HASHS_BOOL},
                tag::I8 => if key_is_int {tag::HASHI_I8}else{tag::HASHS_I8},
                tag::I16 => if key_is_int {tag::HASHI_I16}else{tag::HASHS_I16},
                tag::I32 => if key_is_int {tag::HASHI_I32}else{tag::HASHS_I32},
                tag::I64 => if key_is_int {tag::HASHI_I64}else{tag::HASHS_I64},
                tag::U8 => if key_is_int {tag::HASHI_U8}else{tag::HASHS_U8},
                tag::U16 => if key_is_int {tag::HASHI_U16}else{tag::HASHS_U16},
                tag::U32 => if key_is_int {tag::HASHI_U32}else{tag::HASHS_U32},
                tag::U64 => if key_is_int {tag::HASHI_U64}else{tag::HASHS_U64},
                tag::F32 => if key_is_int {tag::HASHI_F32}else{tag::HASHS_F32},
                tag::F64 => if key_is_int {tag::HASHI_F64}else{tag::HASHS_F64},
                tag::STR => if key_is_int {tag::HASHI_STR}else{tag::HASHS_STR},
                tag::BYTES => if key_is_int {tag::HASHI_BYTES}else{tag::HASHS_BYTES},
                tag::STRUCT => if key_is_int {tag::HASHI_STRUCT}else{tag::HASHS_STRUCT},
                _ => unimplemented!()
            };
    
            head_buf.push(tag);
        }//非空集合

        let len = body.len();
        let len_bytes: [u8; 4] = (len as u32).to_be_bytes();
        head_buf.extend(len_bytes);
        
        self.output.extend(head_buf);
        self.output.extend(body);
        Ok(())
    }
}

impl<'a> ser::SerializeStruct for &'a mut Serializer {
    type Ok = ();
    type Error = Error;

    fn serialize_field<T>(&mut self, key: &'static str, value: &T) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        // println!("-- serialize_field()");
        let _ = key.serialize(&mut **self); //序列化字段名部分
        value.serialize(&mut **self)  //序列化字段值部分
    }

    fn end(self) -> Result<()> 
    {
        // println!("SerializeStruct end(): {:?}", self.output);
        //split_off()函数将self.output从body_index处截断，截断的部分赋值给body
        let body = self.output.split_off(self.body_indexs.pop().unwrap());

        let mut head_buf = Vec::new();
        head_buf.push(tag::STRUCT);
        let struct_name_bytes = self.struct_name.as_bytes();
        let struct_name_len = struct_name_bytes.len() as u8;
        head_buf.push(struct_name_len);             //struct名长度
        head_buf.extend(struct_name_bytes);     //struct名
        head_buf.push(self.field_len as u8);        //字段数量

        let len = body.len(); //struct体的长度
        let len_bytes: [u8; 4] = (len as u32).to_be_bytes();
        head_buf.extend(len_bytes);         //struct体长度

        self.output.extend(head_buf);
        self.output.extend(body);
        Ok(())
    }
}

impl<'a> ser::SerializeStructVariant for &'a mut Serializer {
    type Ok = ();
    type Error = Error;

    fn serialize_field<T>(&mut self, _key: &'static str, _value: &T) -> Result<()>
    where
        T: ?Sized + Serialize,
    {
        unimplemented!()
    }

    fn end(self) -> Result<()> 
    {
        unimplemented!()
    }
}

/// 函数内部将字符串转换为QT端可直接使用的char*指针。
/// 注意，QT端接收到字符串后，要调用rust的回收函数来释放内部的CString
pub fn to_pack_str_ptr(str: &str) -> Vec<u8>
{
    let mut ret: Vec<u8> = Vec::new();
    ret.push(tag::STR_PTR);

    let cstring = CString::new(str).unwrap();  //创建CString（这里会有copy），CString会在原字符串末尾添加'\0'
    let ptr = cstring.as_ptr();    //指向 char* 的指针
    let ptr = ptr as usize; //地址原始位
    let ptr_bytes = ptr.to_be_bytes(); //地址转[u8]数组(地址长度根据当前编译器决定是4字节或8字节)
    
    std::mem::forget(cstring);  //确保cstring不会被回收（QT端要调用qrust_free_bytes函数来释放）

    ret.extend(ptr_bytes);
    ret

    //在这里不会回收cstring
}

/// 函数内部提取向量的指针, 因为不进行向量的序列化操作，操作大长度的向量时效率比from_bytes()高。
/// 注意，Rust端需要保证QT在使用向量期间，该向量不能被Rust回收。
pub fn to_pack_bytes_ptr(v: &[u8]) -> Vec<u8>
{
    let mut ret: Vec<u8> = Vec::new();
    ret.push(tag::BYTES_PTR);

    let len = v.len() as u32; //长度
    let ptr = v.as_ptr();  // &[u8]的地址

    let len_byte4 = len.to_be_bytes();
    let ptr_bytes = (ptr as usize).to_be_bytes();

    ret.extend(len_byte4);
    ret.extend(ptr_bytes);
    ret
}

///与to_pack_bytes_ptr()函数不同的是, 参数v向量在函数内部会克隆一份新的向量，这样QT得到的向量和v向量已解耦(即使v被回收也不影响QT端)
///带来的不便是QT端需要调用Rust释放函数，来释放克隆的向量.
pub fn to_pack_bytes_ptr_clone(v: &[u8]) -> Vec<u8>
{
    let mut ret: Vec<u8> = Vec::new();
    ret.push(tag::BYTES_PTR);

    let v = v.to_vec(); //clone
    let len = v.len() as u32; //长度
    let ptr = v.as_ptr();  // &[u8]的地址

    let len_byte4 = len.to_be_bytes();
    let ptr_bytes = (ptr as usize).to_be_bytes();

    std::mem::forget(v);  //确保函数内部克隆的vec不会被回收

    ret.extend(len_byte4);
    ret.extend(ptr_bytes);
    ret
}

/// 多返回值打包
pub fn to_pack_multi(pack: Vec<u8>) -> Vec<u8>
{
    let mut count: u8 = 0;  //返回值数量
    let body_len = pack.len() as u32;  //返回值体长度

    //计算返回值数量
    let mut p = 0;
    while p < pack.len() 
    {
        match pack[p]
        {
            tag::BOOL | tag::I8 | tag::U8 => 
            {
                if p + 2 > pack.len() {break};
                count += 1;
                p += 2;
            }
            tag::I16 | tag::U16 => 
            {
                if p + 3 > pack.len() {break};
                count += 1;
                p += 3;
            }
            tag::I32 | tag::U32 | tag:: F32 => 
            {
                if p + 5 > pack.len() {break};
                count += 1;
                p += 5;
            }
            tag::I64 | tag::U64 | tag:: F64 => 
            {
                if p + 9 > pack.len() {break};
                count += 1;
                p += 9;
            }
            tag::STR 
            | tag::LIST_BOOL | tag::LIST_I8 | tag::LIST_I16 | tag::LIST_I32 | tag::LIST_I64
            | tag::LIST_U8 | tag::LIST_U16 | tag::LIST_U32 | tag::LIST_U64 | tag::LIST_F32 | tag::LIST_F64 
            | tag::LIST_STR | tag::LIST_STRUCT
            | tag::HASHI_BOOL | tag::HASHI_I8 | tag::HASHI_I16 | tag::HASHI_I32 | tag::HASHI_I64 
            | tag::HASHI_U8 | tag::HASHI_U16 | tag::HASHI_U32 | tag::HASHI_U64 | tag::HASHI_F32 | tag::HASHI_F64 
            | tag::HASHI_STR | tag::HASHI_STRUCT
            | tag::HASHS_BOOL | tag::HASHS_I8 | tag::HASHS_I16 | tag::HASHS_I32 | tag::HASHS_I64 
            | tag::HASHS_U8 | tag::HASHS_U16 | tag::HASHS_U32 | tag::HASHS_U64 | tag::HASHS_F32 | tag::HASHS_F64 
            | tag::HASHS_STR | tag::HASHS_STRUCT =>
            {
                if p + 5 > pack.len() {break};
                let mut byte4: [u8; 4] = [0; 4];
                byte4[0] = pack[p + 1];
                byte4[1] = pack[p + 2];
                byte4[2] = pack[p + 3];
                byte4[3] = pack[p + 4];
                let len = u32::from_be_bytes(byte4) as usize;
                if p + 5 + len > pack.len() {break};
                count += 1;
                p = p + 5 + len;
            }
            tag::STRUCT =>
            {
                if p + 2 > pack.len() {break};
                let name_len = pack[p+1] as usize; //struct名称长度
                if p + 2 + name_len + 5 > pack.len() {break};
                let mut byte4: [u8; 4] = [0; 4];
                byte4[0] = pack[p + 2 + name_len + 1];
                byte4[1] = pack[p + 2 + name_len + 2];
                byte4[2] = pack[p + 2 + name_len + 3];
                byte4[3] = pack[p + 2 + name_len + 4];
                let body_len = u32::from_be_bytes(byte4) as usize;
                if p + 2 + name_len + 5 + body_len > pack.len() {break};
                count += 1;
                p = p + 2 + name_len + 5 + body_len;
            }
            tag::VOID => //空集合(List or Map)
            {
                if p + 5 > pack.len() {break};
                count += 1;
                p += 5;
            }
            _ =>
            {
                error!("Unknown multi rsp tag: {}", pack[p]);
                break;
            }
        }
    }

    //打包
    let mut rsp = Vec::new();
    rsp.push(tag::RET_ARGS);
    rsp.push(count);
    rsp.extend(body_len.to_be_bytes());
    rsp.extend(pack);

    debug!("Multi rsp pack: {:?}", rsp);
    rsp
}