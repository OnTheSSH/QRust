use std::collections::HashMap;
use serde::{Serialize, Deserialize};
use std::{thread, time};
use log::debug;

#[derive(Serialize, Deserialize, Clone, Debug)]
pub struct A {
    a: bool,
    v: Vec<u32>,
}

#[derive(Serialize, Deserialize, Clone, Debug)]
pub struct Base{
    b_true:     bool,
    b_false:    bool,
    i8_min:     i8,
    i8_zero:    i8,
    i8_max:     i8,
    i16_min:    i16,
    i16_zero:   i16,
    i16_max:    i16,
    i32_min:    i32,
    i32_zero:   i32,
    i32_max:    i32,
    i64_min:    i64,
    i64_zero:   i64,
    i64_max:    i64,
    u8_min:     u8,
    u8_max:     u8,
    u16_min:    u16,
    u16_max:    u16,
    u32_min:    u32,
    u32_max:    u32,
    u64_min:    u64,
    u64_max:    u64,
    f32_min:    f32,
    f32_max:    f32,
    f64_min:    f64,
    f64_max:    f64,
    str:        String,
}

#[derive(Serialize, Deserialize, Clone, Debug)]
pub struct ListTypeFields{
    b:      Vec<bool>,
    i8:     Vec<i8>,
    i16:    Vec<i16>,
    i32:    Vec<i32>,
    i64:    Vec<i64>,
    u8:     Vec<u8>,
    u16:    Vec<u16>,
    u32:    Vec<u32>,
    u64:    Vec<u64>,
    f32:    Vec<f32>,
    f64:    Vec<f64>,
    str:    Vec<String>,
}

#[derive(Serialize, Deserialize, Clone, Debug)]
pub struct HashIFields{
    b:      HashMap<i32, bool>,
    i8:     HashMap<i32, i8>,
    i16:    HashMap<i32, i16>,
    i32:    HashMap<i32, i32>,
    i64:    HashMap<i32, i64>,
    u8:     HashMap<i32, u8>,
    u16:    HashMap<i32, u16>,
    u32:    HashMap<i32, u32>,
    u64:    HashMap<i32, u64>,
    f32:    HashMap<i32, f32>,
    f64:    HashMap<i32, f64>,
    str:    HashMap<i32, String>,
}

#[derive(Serialize, Deserialize, Clone, Debug)]
pub struct HashSFields{
    b:      HashMap<String, bool>,
    i8:     HashMap<String, i8>,
    i16:    HashMap<String, i16>,
    i32:    HashMap<String, i32>,
    i64:    HashMap<String, i64>,
    u8:     HashMap<String, u8>,
    u16:    HashMap<String, u16>,
    u32:    HashMap<String, u32>,
    u64:    HashMap<String, u64>,
    f32:    HashMap<String, f32>,
    f64:    HashMap<String, f64>,
    str:    HashMap<String, String>,
}

#[derive(Serialize, Deserialize, Clone, Debug)]
pub struct EmptySet
{
    list_b:         Vec<bool>,
    list_i8:        Vec<i8>,
    list_i16:       Vec<i16>,
    list_i32:       Vec<i32>,
    list_i64:       Vec<i64>,
    list_u8:        Vec<u8>,
    list_u16:       Vec<u16>,
    list_u32:       Vec<u32>,
    list_u64:       Vec<u64>,
    list_f32:       Vec<f32>,
    list_f64:       Vec<f64>,
    list_str:       Vec<String>,

    hi_b:           HashMap<i32, bool>,
    hi_i8:          HashMap<i32, i8>,
    hi_i16:         HashMap<i32, i16>,
    hi_i32:         HashMap<i32, i32>,
    hi_i64:         HashMap<i32, i64>,
    hi_u8:          HashMap<i32, u8>,
    hi_u16:         HashMap<i32, u16>,
    hi_u32:         HashMap<i32, u32>,
    hi_u64:         HashMap<i32, u64>,
    hi_f32:         HashMap<i32, f32>,
    hi_f64:         HashMap<i32, f64>,
    hi_str:         HashMap<i32, String>,

    hs_b:           HashMap<String, bool>,
    hs_i8:          HashMap<String, i8>,
    hs_i16:         HashMap<String, i16>,
    hs_i32:         HashMap<String, i32>,
    hs_i64:         HashMap<String, i64>,
    hs_u8:          HashMap<String, u8>,
    hs_u16:         HashMap<String, u16>,
    hs_u32:         HashMap<String, u32>,
    hs_u64:         HashMap<String, u64>,
    hs_f32:         HashMap<String, f32>,
    hs_f64:         HashMap<String, f64>,
    hs_str:         HashMap<String, String>,
}

pub fn foo()
{
    debug!("foo() running...");
}

pub fn foo1(arg: i32)
{
    debug!("foo1() running... arg: {}", arg);
}

pub fn foo2(arg1: Vec<i32>, arg2: HashMap<i32, String>, arg3: HashMap<String, String>) -> HashMap<String, String>
{
    debug!("foo2() running... arg1: {:?}, arg2:{:?}, args:{:?}", arg1, arg2, arg3);
    arg3
}

pub fn foo_struct(arg: A, count: u8) -> HashMap<String, A>
{
    debug!("foo_struct() runnint... count: {}, arg: {:#?}", count, arg);
    let mut v = HashMap::new();
    for i in 0..count
    {
        v.insert(i.to_string(), arg.clone());
    }
    v
}

pub fn test_base_struct(a1: Base) -> Base
{
    debug!("test_base_struct() runnint... a1: {:#?}", a1);
    a1
}

pub fn test_list_struct(a1: ListTypeFields) -> ListTypeFields
{
    debug!("test_list_struct() runnint... a1: {:?}", a1);
    a1
}

pub fn test_hashi_struct(a1: HashIFields) -> HashIFields
{
    debug!("test_hashi_struct() runnint... a1: {:?}", a1);
    a1
}

pub fn test_hashs_struct(a1: HashSFields) -> HashSFields
{
    debug!("test_hashs_struct() runnint... a1: {:?}", a1);
    a1
}

pub fn test_empty_struct(a1: EmptySet) -> EmptySet
{
    println!("test_empty_struct() runnint... a1: {:?}", a1);
    a1
}

pub fn test_list(b:Vec<bool>, i8:Vec<i8>, i16:Vec<i16>, i32:Vec<i32>, i64:Vec<i64>, 
    u8:Vec<u8>, u16:Vec<u16>, u32:Vec<u32>, u64:Vec<u64>, f32:Vec<f32>, f64:Vec<f64>, 
    str:Vec<String>, strct: Vec<A>) 
    -> (Vec<bool>, Vec<i8>, Vec<i16>, Vec<i32>, Vec<i64>, 
        Vec<u8>, Vec<u16>, Vec<u32>, Vec<u64>, Vec<f32>, Vec<f64>, 
        Vec<String>, Vec<A>) 
{
    debug!("b: {:?}", b);
    debug!("i8: {:?}", i8);
    debug!("i16: {:?}", i16);
    debug!("i32: {:?}", i32);
    debug!("i64: {:?}", i64);
    debug!("u8: {:?}", u8);
    debug!("u16: {:?}", u16);
    debug!("u32: {:?}", u32);
    debug!("u64: {:?}", u64);
    debug!("f32: {:?}", f32);
    debug!("f64: {:?}", f64);
    debug!("str: {:?}", str);
    debug!("strct: {:#?}", strct);

    (b, i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, str, strct)
}

pub fn test_hashi(b:HashMap<i32, bool>, i8:HashMap<i32, i8>, i16:HashMap<i32, i16>, i32:HashMap<i32, i32>, i64:HashMap<i32, i64>, 
    u8:HashMap<i32, u8>, u16:HashMap<i32, u16>, u32:HashMap<i32, u32>, u64:HashMap<i32, u64>, 
    f32:HashMap<i32, f32>, f64:HashMap<i32, f64>, str:HashMap<i32, String>, strct:HashMap<i32, A>) 
    -> 
    (HashMap<i32, bool>, HashMap<i32, i8>, HashMap<i32, i16>, HashMap<i32, i32>, HashMap<i32, i64>, 
        HashMap<i32, u8>, HashMap<i32, u16>, HashMap<i32, u32>, HashMap<i32, u64>, 
        HashMap<i32, f32>, HashMap<i32, f64>, HashMap<i32, String>, HashMap<i32, A>)
{
    debug!("a1: {:?}", b);
    debug!("i8: {:?}", i8);
    debug!("i16: {:?}", i16);
    debug!("i32: {:?}", i32);
    debug!("i64: {:?}", i64);
    debug!("u8: {:?}", u8);
    debug!("u16: {:?}", u16);
    debug!("u32: {:?}", u32);
    debug!("u64: {:?}", u64);
    debug!("f32: {:?}", f32);
    debug!("f64: {:?}", f64);
    debug!("str: {:?}", str);
    debug!("strct: {:#?}", strct);

    (b, i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, str, strct)
}

pub fn test_hashs(b:HashMap<String, bool>, 
    i8:HashMap<String, i8>, i16:HashMap<String, i16>, i32:HashMap<String, i32>, i64:HashMap<String, i64>, 
    u8:HashMap<String, u8>, u16:HashMap<String, u16>, u32:HashMap<String, u32>, u64:HashMap<String, u64>, 
    f32:HashMap<String, f32>, f64:HashMap<String, f64>, str:HashMap<String, String>, strct:HashMap<String, A>) 
    -> (HashMap<String, bool>, 
        HashMap<String, i8>, HashMap<String, i16>, HashMap<String, i32>, HashMap<String, i64>, 
        HashMap<String, u8>, HashMap<String, u16>, HashMap<String, u32>, HashMap<String, u64>, 
        HashMap<String, f32>, HashMap<String, f64>, HashMap<String, String>, HashMap<String, A>)
{
    debug!("a1: {:?}", b);
    debug!("i8: {:?}", i8);
    debug!("i16: {:?}", i16);
    debug!("i32: {:?}", i32);
    debug!("i64: {:?}", i64);
    debug!("u8: {:?}", u8);
    debug!("u16: {:?}", u16);
    debug!("u32: {:?}", u32);
    debug!("u64: {:?}", u64);
    debug!("f32: {:?}", f32);
    debug!("f64: {:?}", f64);
    debug!("str: {:?}", str);
    debug!("strct: {:#?}", strct);

    (b, i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, str, strct)
}

pub fn test_asyn(i: i32) -> i32
{
    debug!("test_asyn() start...");
    //休眠10秒
    let ten_secs = time::Duration::from_millis(10_000);
    thread::sleep(ten_secs);

    debug!("test_asyn() finish...");
    i + i
}

pub fn test_speed(n: u64, i: i32) -> u64
{
    n + i as u64
}