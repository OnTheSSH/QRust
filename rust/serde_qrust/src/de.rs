use crate::{error::{Error, Result}, tag};
use serde::de::{self, Deserialize, DeserializeSeed, MapAccess, SeqAccess, Visitor};
use std::ffi::CStr;
use std::ffi::c_char;
// use log::debug;

pub struct Deserializer<'de> 
{
    input: &'de [u8],
}

impl<'de> Deserializer<'de> 
{
    #[allow(clippy::should_implement_trait)]
    pub fn from_pack(input: &'de [u8]) -> Self 
    {
        Deserializer { input }
    }
}

pub fn from_pack<'a, T>(v: &'a [u8]) -> Result<T>
where
    T: Deserialize<'a>,
{
    let mut deserializer = Deserializer::from_pack(v);
    let t = T::deserialize(&mut deserializer)?;
    if deserializer.input.is_empty() 
    {
        Ok(t)   //解析完成
    } 
    else 
    {
        Err(Error::TrailingCharacters)  //尾随字符无法解析
    }
}

// 基本的解析
impl<'de> Deserializer<'de> 
{
    // 获得input的下一个u8字符, 但不消耗此字符
    fn peek_u8(&mut self) -> Result<u8>
    {
        if self.input.len() < 1 { return Err(Error::Eof) }
        let u = self.input[0];
        Ok(u)
    }

    // 消耗（跳过）n个字节
    fn skip(&mut self, n: usize) -> Result<()>
    {
        if self.input.len() < n { return Err(Error::Eof) }
        self.input = &self.input[n..];
        Ok(())
    }

    // 消耗input的下一个i8字符。
    fn next_i8(&mut self) -> Result<i8> 
    {
        if self.input.len() < 1 { return Err(Error::Eof) }
        let u = self.input[0] as i8;

        self.input = &self.input[1..];
        Ok(u)
    }
    // 消耗input的下一个u8字符。
    fn next_u8(&mut self) -> Result<u8> 
    {
        if self.input.len() < 1 { return Err(Error::Eof) }
        let u = self.input[0];

        self.input = &self.input[1..];
        Ok(u)
    }

    // 消耗input的下两个i8字符, 并返回i16整数。
    fn next_i16(&mut self) -> Result<i16> 
    {
        if self.input.len() < 1 { return Err(Error::Eof) }
        let mut bytes2 = [0u8; 2];
        bytes2[0] = self.input[0];
        bytes2[1] = self.input[1];
        let u2 = i16::from_be_bytes(bytes2);

        self.input = &self.input[2..];
        Ok(u2)
    }
    // 消耗input的下两个u8字符, 并返回u16整数。
    fn next_u16(&mut self) -> Result<u16> 
    {
        if self.input.len() < 1 { return Err(Error::Eof) }
        let mut bytes2 = [0u8; 2];
        bytes2[0] = self.input[0];
        bytes2[1] = self.input[1];
        let u2 = u16::from_be_bytes(bytes2);

        self.input = &self.input[2..];
        Ok(u2)
    }

    // 消耗input的下四个i8字符, 并返回i32整数。
    fn next_i32(&mut self) -> Result<i32> 
    {
        if self.input.len() < 4 { return Err(Error::Eof); }
        
        let mut bytes4 = [0u8; 4];
        bytes4[0] = self.input[0];
        bytes4[1] = self.input[1];
        bytes4[2] = self.input[2];
        bytes4[3] = self.input[3];
        let u4 = i32::from_be_bytes(bytes4);

        self.input = &self.input[4..];
        Ok(u4)
    }
    // 消耗input的下四个u8字符, 并返回u32整数。
    fn next_u32(&mut self) -> Result<u32> 
    {
        if self.input.len() < 4 { return Err(Error::Eof); }
        
        let mut bytes4 = [0u8; 4];
        bytes4[0] = self.input[0];
        bytes4[1] = self.input[1];
        bytes4[2] = self.input[2];
        bytes4[3] = self.input[3];
        let u4 = u32::from_be_bytes(bytes4);

        self.input = &self.input[4..];
        Ok(u4)
    }

    // 消耗input的下八个i8字符, 并返回i64整数。
    fn next_i64(&mut self) -> Result<i64> 
    {
        if self.input.len() < 4 { return Err(Error::Eof); }
        
        let mut bytes8 = [0u8; 8];
        bytes8[0] = self.input[0];
        bytes8[1] = self.input[1];
        bytes8[2] = self.input[2];
        bytes8[3] = self.input[3];
        bytes8[4] = self.input[4];
        bytes8[5] = self.input[5];
        bytes8[6] = self.input[6];
        bytes8[7] = self.input[7];
        let u8 = i64::from_be_bytes(bytes8);

        self.input = &self.input[8..];
        Ok(u8)
    }
    // 消耗input的下八个u8字符, 并返回u64整数。
    fn next_u64(&mut self) -> Result<u64> 
    {
        if self.input.len() < 4 { return Err(Error::Eof); }
        
        let mut bytes8 = [0u8; 8];
        bytes8[0] = self.input[0];
        bytes8[1] = self.input[1];
        bytes8[2] = self.input[2];
        bytes8[3] = self.input[3];
        bytes8[4] = self.input[4];
        bytes8[5] = self.input[5];
        bytes8[6] = self.input[6];
        bytes8[7] = self.input[7];
        let u8 = u64::from_be_bytes(bytes8);

        self.input = &self.input[8..];
        Ok(u8)
    }
}

impl<'de, 'a> de::Deserializer<'de> for &'a mut Deserializer<'de> 
{
    type Error = Error;

    // 查看输入数据以决定要将其反序列化为 Serde 数据模型类型。并非所有数据格式都能支持此操作。
    // 支持 `deserialize_any` 的格式称为自描述格式。
    fn deserialize_any<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("== deserialize_any");
        match self.peek_u8()? 
        {
            tag::BOOL   =>  self.deserialize_bool(visitor),
            tag::I8     =>  self.deserialize_i8(visitor),
            tag::I16    =>  self.deserialize_i16(visitor),
            tag::I32    =>  self.deserialize_i32(visitor),
            tag::I64    =>  self.deserialize_i64(visitor),
            tag::U8     =>  self.deserialize_u8(visitor),
            tag::U16    =>  self.deserialize_u16(visitor),
            tag::U32    =>  self.deserialize_u32(visitor),
            tag::U64    =>  self.deserialize_u64(visitor),
            tag::F32    =>  self.deserialize_f32(visitor),
            tag::F64    =>  self.deserialize_f64(visitor),
            tag::STR    =>  self.deserialize_str(visitor),
            tag::BYTES  =>  self.deserialize_bytes(visitor),
            // tag::STRUCT =>  self.deserialize_struct(visitor),
            tag::LIST_BOOL | tag::LIST_I8 | tag::LIST_I16 | tag::LIST_I32 | tag::LIST_I64
                | tag::LIST_U8 | tag::LIST_U16 | tag::LIST_U32 | tag::LIST_U64 | tag::LIST_F32 | tag::LIST_F64
                | tag::LIST_STR | tag::LIST_STRUCT => self.deserialize_seq(visitor),
            tag::HASHI_BOOL | tag::HASHI_I8 | tag::HASHI_I16 | tag::HASHI_I32 | tag::HASHI_I64
                | tag::HASHI_U8 | tag::HASHI_U16 | tag::HASHI_U32 | tag::HASHI_U64 | tag::HASHI_F32 | tag::HASHI_F64
                | tag::HASHI_STR | tag::HASHI_STRUCT 
                | tag::HASHS_BOOL | tag::HASHS_I8 | tag::HASHS_I16 | tag::HASHS_I32 | tag::HASHS_I64
                | tag::HASHS_U8 | tag::HASHS_U16 | tag::HASHS_U32 | tag::HASHS_U64 | tag::HASHS_F32 | tag::HASHS_F64
                | tag::HASHS_STR | tag::HASHS_STRUCT => self.deserialize_map(visitor),
            _ => Err(Error::Syntax),
        }
    }

    fn deserialize_bool<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // debug!("** deserialize_bool()");
        // debug!("bool input: {:?}", self.input);
        if self.input.len() < 2 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::BOOL { return Err(Error::ExpectedBool); }
        let val = self.next_u8().unwrap();  //值
        if val == 1 
        {
            visitor.visit_bool(true)
        }
        else if val == 0 
        {
            visitor.visit_bool(false)
        }
        else 
        {
            Err(Error::ExpectedBool)
        }
    }

    fn deserialize_i8<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_i8()");
        if self.input.len() < 2 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::I8 { return Err(Error::ExpectedI8); }
        let val = self.next_i8().unwrap();  //值
        visitor.visit_i8(val)
    }

    fn deserialize_i16<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_i16()");
        if self.input.len() < 3 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::I16 { return Err(Error::ExpectedI16); }
        let val = self.next_i16().unwrap();  //值
        visitor.visit_i16(val)
    }

    fn deserialize_i32<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_i32()");
        if self.input.len() < 5 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::I32 { return Err(Error::ExpectedI32); }
        let val = self.next_i32().unwrap();  //值
        visitor.visit_i32(val)
    }

    fn deserialize_i64<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_i64()");
        if self.input.len() < 9 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::I64 { return Err(Error::ExpectedI64); }
        let val = self.next_i64().unwrap();  //值
        visitor.visit_i64(val)
    }

    fn deserialize_u8<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_u8()");
        if self.input.len() < 2 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::U8 { return Err(Error::ExpectedU8); }
        let val = self.next_u8().unwrap();  //值
        visitor.visit_u8(val)
    }

    fn deserialize_u16<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_u16()");
        if self.input.len() < 3 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::U16 { return Err(Error::ExpectedU16); }
        let val = self.next_u16().unwrap();  //值
        visitor.visit_u16(val)
    }

    fn deserialize_u32<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_u32()");
        if self.input.len() < 5 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::U32 { return Err(Error::ExpectedU32); }
        let val = self.next_u32().unwrap();  //值
        visitor.visit_u32(val)
    }

    fn deserialize_u64<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_u64()");
        if self.input.len() < 9 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::U64 { return Err(Error::ExpectedU64); }
        let val = self.next_u64().unwrap();  //值
        visitor.visit_u64(val)
    }

    fn deserialize_f32<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_f32()");
        if self.input.len() < 5 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::F32 { return Err(Error::ExpectedF32); }
        let mut bytes: [u8; 4] = [0; 4];
        bytes[0] = self.next_u8().unwrap();
        bytes[1] = self.next_u8().unwrap();
        bytes[2] = self.next_u8().unwrap();
        bytes[3] = self.next_u8().unwrap();
        // debug!("deserialize_f32 {} {} {} {}", bytes[0], bytes[1], bytes[2], bytes[3]);
        let val = f32::from_be_bytes(bytes); //值
        visitor.visit_f32(val)
    }

    fn deserialize_f64<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_f64()");
        if self.input.len() < 9 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::F64 { return Err(Error::ExpectedF64); }
        let mut bytes: [u8; 8] = [0; 8];
        bytes[0] = self.next_u8().unwrap();
        bytes[1] = self.next_u8().unwrap();
        bytes[2] = self.next_u8().unwrap();
        bytes[3] = self.next_u8().unwrap();
        bytes[4] = self.next_u8().unwrap();
        bytes[5] = self.next_u8().unwrap();
        bytes[6] = self.next_u8().unwrap();
        bytes[7] = self.next_u8().unwrap();
        let val = f64::from_be_bytes(bytes); //值
        visitor.visit_f64(val)
    }

    fn deserialize_char<V>(self, _visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        unimplemented!()
    }

    fn deserialize_str<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_str()");
        if self.input.len() < 5 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::STR { return Err(Error::ExpectedString); }
        let len = self.next_u32().unwrap();  //长度
        if self.input.len() < len as usize { return Err(Error::ExpectedString); }

        let val: &'de str = match std::str::from_utf8(&self.input[..len as usize])
        {
            Ok(s) => s,
            Err(_) => return Err(Error::ExpectedString),
        };

        // &str长度(utf8)和self.input的字节长度(u8)并不相同
        self.skip(len as usize)?;
        visitor.visit_borrowed_str(val)
    }

    fn deserialize_string<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_string()");
        self.deserialize_str(visitor)
    }

    fn deserialize_bytes<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_bytes()");
        if self.input.len() < 5 { return Err(Error::Eof); }
        let tag = self.next_u8().unwrap();  //tag
        if tag != tag::BYTES { return Err(Error::ExpectedBytes); }
        let len = self.next_u32().unwrap();  //长度
        if self.input.len() < len as usize { return Err(Error::ExpectedBytes); }
        let val: &'de [u8] = &self.input[..len as usize]; //值
        // println!("val: {:?}", val);

        let _ = self.skip(val.len());
        visitor.visit_borrowed_bytes(val)
    }

    fn deserialize_byte_buf<V>(self, _visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        unimplemented!()
    }

    fn deserialize_option<V>(self, _visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        unimplemented!()
    }

    fn deserialize_unit<V>(self, _visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        unimplemented!()
    }

    fn deserialize_unit_struct<V>(self, _name: &'static str, _visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        unimplemented!()
    }

    fn deserialize_newtype_struct<V>(self, _name: &'static str, _visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        unimplemented!()
    }

    fn deserialize_seq<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_seq()");
        if self.input.len() < 5 { return Err(Error::Eof); }
        let list_tag = self.next_u8().unwrap(); //list tag
        if list_tag < 50 || list_tag >= 100 { return Err(Error::ExpectedList); }
        let list_len = self.next_u32().unwrap(); //list 长度
        if self.input.len() < list_len as usize { return Err(Error::ExpectedList); }
        //允许访问者访问序列中的每个元素。
        let value = visitor.visit_seq(
            ItemSeparated::new(self, list_len as usize, self.input.len()))?;
        Ok(value)
    }

    fn deserialize_tuple<V>(self, _len: usize, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // debug!("** deserialize_tuple()");
        if self.input.len() < 6 { return Err(Error::Eof); }
        let args_tag = self.next_u8().unwrap(); //args tag
        if args_tag != tag::FUN_ARGS { return Err(Error::ExpectedArgs); }
        let _num = self.next_u8()?; //args 数量
        // debug!("tuple数量: {}", num);
        let args_len = self.next_u32().unwrap(); //args 长度
        // debug!("tuple长度: {} {:?}", args_len, self.input);
        if self.input.len() < args_len as usize { return Err(Error::ExpectedArgsEof); }
        //允许访问者访问Tuple中的每个元素
        let value = visitor.visit_seq(
            ItemSeparated::new(self, args_len as usize, self.input.len()))?;
        Ok(value)
    }

    fn deserialize_tuple_struct<V>(self, _name: &'static str, _len: usize, _visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        unimplemented!()
    }

    fn deserialize_map<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_map()");
        if self.input.len() < 5 { return Err(Error::Eof); }
        let _tag = self.next_u8();
        let map_len = self.next_u32().unwrap(); //map 长度
        if self.input.len() < map_len as usize { return Err(Error::ExpectedMap); }

        let value = visitor.visit_map(
            ItemSeparated::new(self, map_len as usize, self.input.len()))?;
        Ok(value)
    }

    fn deserialize_struct<V>(self, _name: &'static str, _fields: &'static [&'static str], visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // debug!("** deserialize_struct()");
        if self.input.len() < 2 { return Err(Error::Eof); }
        let struct_tag = self.next_u8().unwrap(); //struct tag
        if tag::STRUCT != struct_tag { return Err(Error::ExpectedStruct); }
        let name_len = self.next_u8().unwrap(); //struct名称长度
        if self.input.len() < name_len as usize { return Err(Error::ExpectedStructEof); }
        let _ = self.skip(name_len as usize); //跳过struct名称
        if self.input.len() < 5 { return Err(Error::ExpectedStructEof); }
        let num = self.next_u8().unwrap();    //字段数量
        let len = self.next_u32().unwrap();     //struct体长度

        // debug!("num: {}, len: {}, {:?}", num, len, self.input);
        let value = visitor.visit_map(StructSeparated::new(self, num, len as i32))?;
        Ok(value)
    }

    fn deserialize_enum<V>(self, _name: &'static str, _variants: &'static [&'static str], _visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        unimplemented!()
    }

    //Serde中的标识符是标识结构体字段或枚举变体的类型。在JSON中，结构字段和枚举变量表示为字符串。
    //在其他格式中，它们可以表示为数字索引。
    fn deserialize_identifier<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("** deserialize_identifier()");
        self.deserialize_str(visitor)
    }

    //类似于'deserialize_any'，但向'Deserializer'指示调用哪个'Visitor'方法没有区别，因为数据将被忽略。
    //
    //一些反序列化器能够比'deserialize_any'更有效地实现此功能，例如，通过快速跳过匹配的分隔符而无需密切关注中间的数据。
    //
    //某些格式根本无法实现此功能。可以实现'deserialize_any'和'deserialize_ignored_any'的格式被称为自描述。
    fn deserialize_ignored_any<V>(self, visitor: V) -> Result<V::Value>
    where
        V: Visitor<'de>,
    {
        // println!("== deserialize_ignored_any");

        self.deserialize_any(visitor)
    }
}

/////////////////////////////// List | Map | Tuple Separated //////////////////////////////////////

struct ItemSeparated<'a, 'de: 'a> 
{
    de: &'a mut Deserializer<'de>,
    list_body_len:  usize,  //集合体的长度
    old_input_len:  usize,  //解析集合体前，input的长度(当 input长度 + list_body_len >= old_input_len 时，解析完成)
}

impl<'a, 'de> ItemSeparated<'a, 'de> 
{
    fn new(de: &'a mut Deserializer<'de>, list_body_len: usize, old_input_len: usize) -> Self 
    {
        ItemSeparated { de, list_body_len, old_input_len }
    }
}

//为' Visitor '提供' SeqAccess '，使其能够遍历list中的元素。
impl<'de, 'a> SeqAccess<'de> for ItemSeparated<'a, 'de> 
{
    type Error = Error;

    fn next_element_seed<T>(&mut self, seed: T) -> Result<Option<T::Value>>
    where
        T: DeserializeSeed<'de>,
    {        
        if self.de.input.len() + self.list_body_len == self.old_input_len
        {
            return Ok(None);    //已经没有更多的元素。
        }
        if self.de.input.len() + self.list_body_len < self.old_input_len
        {
            return Err(Error::ExpectedListEof)
        }

        // debug!("list SeqAccess: {:?}", self.de.input);
        //反序列化元素
        seed.deserialize(&mut *self.de).map(Some)
    }
}

//为' Visitor '提供' MapAccess '，使其能够遍历map中的元素
impl<'de, 'a> MapAccess<'de> for ItemSeparated<'a, 'de> 
{
    type Error = Error;

    fn next_key_seed<K>(&mut self, seed: K) -> Result<Option<K::Value>>
    where
        K: DeserializeSeed<'de>,
    {        
        if self.de.input.len() + self.list_body_len == self.old_input_len
        {
            return Ok(None);    //已经没有更多的元素。
        }
        if self.de.input.len() + self.list_body_len < self.old_input_len
        {
            return Err(Error::ExpectedMapEof)
        }

        let key_type = self.de.input[0];
        if key_type != tag::I32 && key_type != tag::STR
        {
            return Err(Error::ExpectedMapKeyType)  //key类型不支持
        }

        //反序列化map键
        seed.deserialize(&mut *self.de).map(Some)
    }

    fn next_value_seed<V>(&mut self, seed: V) -> Result<V::Value>
    where
        V: DeserializeSeed<'de>,
    {
        //反序列化value
        seed.deserialize(&mut *self.de)
    }

}

/////////////////////////////// struct Separated //////////////////////////////////////
struct StructSeparated<'a, 'de: 'a> 
{
    de: &'a mut Deserializer<'de>,
    num: u8,    //struct的剩余字段数量
    len: i32,   //struct体的剩余长度
}

impl<'a, 'de> StructSeparated<'a, 'de> 
{
    fn new(de: &'a mut Deserializer<'de>, num: u8, len: i32) -> Self 
    {
        StructSeparated { de, num, len }
    }
}

//为' Visitor '提供' MapAccess '，使其能够遍历struct中的字段
impl<'de, 'a> MapAccess<'de> for StructSeparated<'a, 'de> 
{
    type Error = Error;

    //解析字段名
    fn next_key_seed<K>(&mut self, seed: K) -> Result<Option<K::Value>>
    where
        K: DeserializeSeed<'de>,
    {  
        if self.len == 0 || self.num == 0
        {
            return Ok(None);    //已经没有更多的元素。
        }
        if self.len < 0
        {
            return Err(Error::ExpectedStructEof)
        }

        let filed_name_len = self.de.input[1]; //字段名称长度
        if self.de.input.len() < filed_name_len as usize
        {
            return Err(Error::ExpectedStructEof)
        }

        self.num -= 1; //消耗字段数量
        self.len = self.len - 5 - filed_name_len as i32;   //消耗字段名称字节数

        // debug!("反序列化字段名");
        //反序列化字段名
        seed.deserialize(&mut *self.de).map(Some)
    }

    //解析字段值
    fn next_value_seed<V>(&mut self, seed: V) -> Result<V::Value>
    where
        V: DeserializeSeed<'de>,
    {
        // println!("*** struct next_value_seed()");
        //计算消耗的字节数
        match self.de.input[0]
        {
            tag::BOOL  => self.len -= 2,
            tag::I8    => self.len -= 2,
            tag::I16   => self.len -= 3,
            tag::I32   => self.len -= 5,
            tag::I64   => self.len -= 9,
            tag::U8    => self.len -= 2,
            tag::U16   => self.len -= 3,
            tag::U32   => self.len -= 5,
            tag::U64   => self.len -= 9,
            tag::F32   => self.len -= 5,
            tag::F64   => self.len -= 9,
            tag::STR | tag::BYTES => {
                if self.de.input.len() < 5 { return Err(Error::ExpectedStructEof) }
                let mut bytes: [u8; 4] = [0; 4];
                bytes[0] = self.de.input[1];
                bytes[1] = self.de.input[2];
                bytes[2] = self.de.input[3];
                bytes[3] = self.de.input[4];
                let len = u32::from_be_bytes(bytes);
                self.len = self.len - 1 - 4 - len as i32;
            }
            tag::LIST_BOOL | tag::LIST_I8 | tag::LIST_I16 | tag::LIST_I32 | tag::LIST_I64
                | tag::LIST_U8 | tag::LIST_U16 | tag::LIST_U32 | tag::LIST_U64
                | tag::LIST_F32 | tag::LIST_F64 | tag::LIST_STR | tag::LIST_BYTES =>
            {
                let mut bytes: [u8; 4] = [0; 4];
                bytes[0] = self.de.input[1];
                bytes[1] = self.de.input[2];
                bytes[2] = self.de.input[3];
                bytes[3] = self.de.input[4];
                let len = u32::from_be_bytes(bytes);
                self.len = self.len - 1 - 4 - len as i32;
            }
            tag::HASHI_BOOL | tag::HASHI_I8 | tag::HASHI_I16 | tag::HASHI_I32 | tag::HASHI_I64
                | tag::HASHI_U8 | tag::HASHI_U16 | tag::HASHI_U32 | tag::HASHI_U64
                | tag::HASHI_F32 | tag::HASHI_F64 | tag::HASHI_STR | tag::HASHI_BYTES =>
            {
                let mut bytes: [u8; 4] = [0; 4];
                bytes[0] = self.de.input[1];
                bytes[1] = self.de.input[2];
                bytes[2] = self.de.input[3];
                bytes[3] = self.de.input[4];
                let len = u32::from_be_bytes(bytes);
                self.len = self.len - 1 - 4 - len as i32;
            }
            tag::HASHS_BOOL | tag::HASHS_I8 | tag::HASHS_I16 | tag::HASHS_I32 | tag::HASHS_I64
                | tag::HASHS_U8 | tag::HASHS_U16 | tag::HASHS_U32 | tag::HASHS_U64
                | tag::HASHS_F32 | tag::HASHS_F64 | tag::HASHS_STR | tag::HASHS_BYTES =>
            {
                let mut bytes: [u8; 4] = [0; 4];
                bytes[0] = self.de.input[1];
                bytes[1] = self.de.input[2];
                bytes[2] = self.de.input[3];
                bytes[3] = self.de.input[4];
                let len = u32::from_be_bytes(bytes);
                self.len = self.len - 1 - 4 - len as i32;
            }
            _ => { return Err(Error::ExpectedList) }
        }

        //反序列化value
        seed.deserialize(&mut *self.de)
    }
}

/// String指针
pub fn from_str_ptr<'a>(pack: &'a [u8]) -> Option<&'a str>
{
    if pack.len() == 0 { return None; }
    if pack[0] != tag::STR_PTR { return None; }

    let len = if usize::BITS == 64 { 8 } else { 4 };
    if 1 + len as usize > pack.len() { return None; }

    //获得序列化的指针地址（4字节或8字节）
    let ptr_usize: usize;
    if len == 8
    {
        let mut bytes8 = [0u8; 8];
        bytes8[0] = pack[1];
        bytes8[1] = pack[2];
        bytes8[2] = pack[3];
        bytes8[3] = pack[4];
        bytes8[4] = pack[5];
        bytes8[5] = pack[6];
        bytes8[6] = pack[7];
        bytes8[7] = pack[8];

        ptr_usize = u64::from_be_bytes(bytes8) as usize;
    }
    else 
    {
        let mut bytes4 = [0u8; 4];
        bytes4[0] = pack[1];
        bytes4[1] = pack[2];
        bytes4[2] = pack[3];
        bytes4[3] = pack[4];

        ptr_usize = u32::from_be_bytes(bytes4) as usize;
    }

    //反序列化
    let ptr_c_str = ptr_usize as *const c_char;
    let c_str = unsafe{ CStr::from_ptr(ptr_c_str) };
    match c_str.to_str()
    {
        Ok(s) => Some(s),
        Err(_) => None,
    }
}

// bytes指针
pub fn to_bytes_ptr<'a>(pack: &'a [u8]) -> Option<&'a [u8]>
{
    if pack.len() < 5 { return None; }
    if pack[0] != tag::BYTES_PTR { return None; }

    //获得数据长度
    let mut bytes4 = [0u8; 4];
    bytes4[0] = pack[1];
    bytes4[1] = pack[2];
    bytes4[2] = pack[3];
    bytes4[3] = pack[4];
    let len = u32::from_be_bytes(bytes4);

    let bits_len = if usize::BITS == 64 { 8 } else { 4 };
    if 5 + bits_len as usize > pack.len() { return None; }

    //获得序列化的指针地址（4字节或8字节）
    let ptr_usize: usize;
    if bits_len == 8
    {
        let mut bytes8 = [0u8; 8];
        bytes8[0] = pack[5];
        bytes8[1] = pack[6];
        bytes8[2] = pack[7];
        bytes8[3] = pack[8];
        bytes8[4] = pack[9];
        bytes8[5] = pack[10];
        bytes8[6] = pack[11];
        bytes8[7] = pack[12];

        ptr_usize = u64::from_be_bytes(bytes8) as usize;
    }
    else 
    {
        let mut bytes4 = [0u8; 4];
        bytes4[0] = pack[5];
        bytes4[1] = pack[6];
        bytes4[2] = pack[7];
        bytes4[3] = pack[8];

        ptr_usize = u32::from_be_bytes(bytes4) as usize;
    }

    //反序列化
    let ptr_u8 = ptr_usize as *const u8;
    let ret = unsafe{ std::slice::from_raw_parts(ptr_u8, len as usize) };
    Some(ret)
}