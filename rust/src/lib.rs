
mod api;
use api::test_speed;
use serde_qrust::{de, ser, tag, error::{Error, Result}};

/// ////////////////////////////////////////////////////////////////
/// 手动的运行时反射函数调用，您需要在这里：
///     1）根据函数名，匹配(match)对应的函数
///     2）执行函数前，需要调用from_pack() 解析出函数的参数（反序列化）
///     3）执行函数后，需要调用to_pack()打包返回值（序列化）
/// Manual runtime reflection function call, you need here:
///     1) match the corresponding function according to the function name
///     2) Before executing the function, you need to call from_pack() to 
///        parse out the parameters of the function (deserialization).
///     3) After executing the function, you need to call to_pack() to pack 
///        the return value (serialization).
/// ///////////////////////////////////////////////////////////////
fn invoke(fun_name: &str, mut args: Vec<&[u8]>) -> Result<Option<Vec<u8>>>
{
    match fun_name
    {
        "foo" =>    //无参数、无返回的函数调用示例 Example of a function call with no arguments and no returns
        {
            api::foo();     //调用函数  Calling function
            Ok(None)
        } 
        "foo1" =>   //有1个参数、无返回的函数调用示例  Example of a function call with one argument and no return
        {
            let a1 = de::from_pack(args.pop().unwrap())?; //反序列化获得参数  Deserialization gets the parameters
            api::foo1(a1);  
            Ok(None)
        }
        "foo2" =>   //有多个参数、有返回的函数调用示例  There are multiple arguments and examples of function calls that are returned
        {
            let a1 = de::from_pack(args.pop().unwrap())?;
            let a2 = de::from_pack(args.pop().unwrap())?;
            let a3 = de::from_pack(args.pop().unwrap())?;

            let ret = api::foo2(a1, a2, a3); 
            let pack = ser::to_pack(&ret)?;     //返回值打包（序列化） Return value packaging (serialization)
            Ok(Some(pack))
        }
        "foo_struct" => //自定义struct类型的示例  Example of a custom struct type
        {
            let arg = de::from_pack(args.pop().unwrap())?;
            let count = de::from_pack(args.pop().unwrap())?;

            let ret = api::foo_struct(arg, count);
            let pack = ser::to_pack(&ret)?; 
            Ok(Some(pack))
        }

        "test_base_struct" => 
        {
            let a1 = de::from_pack(args.pop().unwrap())?;

            let ret = api::test_base_struct(a1);
            let pack = ser::to_pack(&ret)?;
            Ok(Some(pack))
        }
        "test_list_struct" => 
        {
            let a1 = de::from_pack(args.pop().unwrap())?;

            let ret = api::test_list_struct(a1);
            let pack = ser::to_pack(&ret)?;
            Ok(Some(pack))
        }
        "test_hashi_struct" => 
        {
            let a1 = de::from_pack(args.pop().unwrap())?;

            let ret = api::test_hashi_struct(a1);
            let pack = ser::to_pack(&ret)?;
            Ok(Some(pack))
        }
        "test_hashs_struct" => 
        {
            let a1 = de::from_pack(args.pop().unwrap())?;

            let ret = api::test_hashs_struct(a1);
            let pack = ser::to_pack(&ret)?;
            Ok(Some(pack))
        }
        "test_empty_struct" => 
        {
            let a1 = de::from_pack(args.pop().unwrap())?;
            let ret = api::test_empty_struct(a1);
            let pack = ser::to_pack(&ret)?;
            Ok(Some(pack))
        }
        "test_list" => 
        {
            let b = de::from_pack(args.pop().unwrap())?;
            let i8 = de::from_pack(args.pop().unwrap())?;
            let i16 = de::from_pack(args.pop().unwrap())?;
            let i32 = de::from_pack(args.pop().unwrap())?;
            let i64 = de::from_pack(args.pop().unwrap())?;
            let u8 = de::from_pack(args.pop().unwrap())?;
            let u16 = de::from_pack(args.pop().unwrap())?;
            let u32 = de::from_pack(args.pop().unwrap())?;
            let u64 = de::from_pack(args.pop().unwrap())?;
            let f32 = de::from_pack(args.pop().unwrap())?;
            let f64 = de::from_pack(args.pop().unwrap())?;
            let str = de::from_pack(args.pop().unwrap())?;
            let strct = de::from_pack(args.pop().unwrap())?;

            let (b, i8, i16, i32, i64, 
                u8, u16, u32, u64, 
                f32, f64, str, strct) 
                = api::test_list(b, i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, str, strct);

            //函数返回多个值  
            //Function returns multiple values
            let mut pack = Vec::new();
            pack.extend(ser::to_pack(&b)?);
            pack.extend(ser::to_pack(&i8)?);
            pack.extend(ser::to_pack(&i16)?);
            pack.extend(ser::to_pack(&i32)?);
            pack.extend(ser::to_pack(&i64)?);
            pack.extend(ser::to_pack(&u8)?);
            pack.extend(ser::to_pack(&u16)?);
            pack.extend(ser::to_pack(&u32)?);
            pack.extend(ser::to_pack(&u64)?);
            pack.extend(ser::to_pack(&f32)?);
            pack.extend(ser::to_pack(&f64)?);
            pack.extend(ser::to_pack(&str)?);
            pack.extend(ser::to_pack(&strct)?);
            let pack_multi = ser::to_pack_multi(pack);
            Ok(Some(pack_multi))
        }
        "test_hashi" =>
        {
            let b = de::from_pack(args.pop().unwrap())?;
            let i8 = de::from_pack(args.pop().unwrap())?;
            let i16 = de::from_pack(args.pop().unwrap())?;
            let i32 = de::from_pack(args.pop().unwrap())?;
            let i64 = de::from_pack(args.pop().unwrap())?;
            let u8 = de::from_pack(args.pop().unwrap())?;
            let u16 = de::from_pack(args.pop().unwrap())?;
            let u32 = de::from_pack(args.pop().unwrap())?;
            let u64 = de::from_pack(args.pop().unwrap())?;
            let f32 = de::from_pack(args.pop().unwrap())?;
            let f64 = de::from_pack(args.pop().unwrap())?;
            let str = de::from_pack(args.pop().unwrap())?;
            let strct = de::from_pack(args.pop().unwrap())?;

            let (b, i8, i16, i32, i64, 
                u8, u16, u32, u64, 
                f32, f64, str, strct) 
                = api::test_hashi(b, i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, str, strct);
            
            //函数返回多个值
            //Function returns multiple values
            let mut pack = Vec::new();
            pack.extend(ser::to_pack(&b)?);
            pack.extend(ser::to_pack(&i8)?);
            pack.extend(ser::to_pack(&i16)?);
            pack.extend(ser::to_pack(&i32)?);
            pack.extend(ser::to_pack(&i64)?);
            pack.extend(ser::to_pack(&u8)?);
            pack.extend(ser::to_pack(&u16)?);
            pack.extend(ser::to_pack(&u32)?);
            pack.extend(ser::to_pack(&u64)?);
            pack.extend(ser::to_pack(&f32)?);
            pack.extend(ser::to_pack(&f64)?);
            pack.extend(ser::to_pack(&str)?);
            pack.extend(ser::to_pack(&strct)?);
            let pack_multi = ser::to_pack_multi(pack);
            Ok(Some(pack_multi))
        }
        "test_hashs" =>
        {
            let b = de::from_pack(args.pop().unwrap())?;
            let i8 = de::from_pack(args.pop().unwrap())?;
            let i16 = de::from_pack(args.pop().unwrap())?;
            let i32 = de::from_pack(args.pop().unwrap())?;
            let i64 = de::from_pack(args.pop().unwrap())?;
            let u8 = de::from_pack(args.pop().unwrap())?;
            let u16 = de::from_pack(args.pop().unwrap())?;
            let u32 = de::from_pack(args.pop().unwrap())?;
            let u64 = de::from_pack(args.pop().unwrap())?;
            let f32 = de::from_pack(args.pop().unwrap())?;
            let f64 = de::from_pack(args.pop().unwrap())?;
            let str = de::from_pack(args.pop().unwrap())?;
            let strct = de::from_pack(args.pop().unwrap())?;

            let (b, i8, i16, i32, i64, 
                u8, u16, u32, u64, 
                f32, f64, str, strct) 
                = api::test_hashs(b, i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, str, strct);

            let mut pack = Vec::new();
            pack.extend(ser::to_pack(&b)?);
            pack.extend(ser::to_pack(&i8)?);
            pack.extend(ser::to_pack(&i16)?);
            pack.extend(ser::to_pack(&i32)?);
            pack.extend(ser::to_pack(&i64)?);
            pack.extend(ser::to_pack(&u8)?);
            pack.extend(ser::to_pack(&u16)?);
            pack.extend(ser::to_pack(&u32)?);
            pack.extend(ser::to_pack(&u64)?);
            pack.extend(ser::to_pack(&f32)?);
            pack.extend(ser::to_pack(&f64)?);
            pack.extend(ser::to_pack(&str)?);
            pack.extend(ser::to_pack(&strct)?);
            let pack_multi = ser::to_pack_multi(pack);
            Ok(Some(pack_multi))
        }
        "test_asyn" =>
        {
            //在rust端，异步调用没有特殊性
            //On the rust side, there is nothing special about asynchronous calls
            let i = de::from_pack(args.pop().unwrap())?;
            let ret = api::test_asyn(i);
            let pack = ser::to_pack(&ret)?;
            Ok(Some(pack))
        }
        "test_speed" =>
        {
            let n = de::from_pack(args.pop().unwrap())?;
            let i = de::from_pack(args.pop().unwrap())?;
            let n = test_speed(n, i);
            let pack = ser::to_pack(&n)?;
            Ok(Some(pack))
        }
        _ =>
        {
            Err(Error::ExpectedFun)
        }
    }
}

/// //////////////////////////////////////////////////////////////////////
/// log4rs日志初始化，您可以改为自定义的日志形式
/// log4rs initializes logs. You can change the log format to a custom one
/// //////////////////////////////////////////////////////////////////////
use log::{debug, error};
use log::LevelFilter;
use log4rs;
use log4rs::encode::pattern::PatternEncoder;
use log4rs::config::{Appender, Config, Root};
use log4rs::append::rolling_file::policy::compound::roll::fixed_window::FixedWindowRoller;
use log4rs::append::rolling_file::policy::compound::trigger::size::SizeTrigger;
use log4rs::append::rolling_file::policy::compound::CompoundPolicy;
use log4rs::filter::threshold::ThresholdFilter;
use log4rs::append::rolling_file::RollingFileAppender;
use std::path::Path;
use std::env;
fn log_init(lvl: LevelFilter, log_path: &str)
{
    let log_file = "qrust2.log";
    let backup_log_file = "qrust2_{}.log";
    //日志文件路径    Log file path
    let log_path = Path::new(log_path).to_owned();     
    //日志文件   Log file
    let mut file_path = log_path.clone();
    file_path.push(log_file);
    let filename = file_path.to_str().unwrap();
    let filename = filename.replace("\\", "/");
    //备份文件  Backup file
    let mut bak_path = log_path.clone();
    bak_path.push(backup_log_file);
    let bakfilename = bak_path.to_str().unwrap();
    let bakfilename = bakfilename.replace("\\", "/");

    //备份文件数量  Number of backup files
    let window_size = 5; 
    let fixed_window_roller = 
        FixedWindowRoller::builder().build(&bakfilename, window_size).unwrap();

    //log文件大小   log file size
    let size_limit = 5 * 1024 * 1024; // 最大5M  Max 5M
    let size_trigger = SizeTrigger::new(size_limit);

    let compound_policy = CompoundPolicy::new(Box::new(size_trigger),Box::new(fixed_window_roller));

    let file = RollingFileAppender::builder()
        .encoder(Box::new(PatternEncoder::new("{d(%Y-%m-%d %H:%M:%S)} {l} - {m}{n}")))
        .build(filename, Box::new(compound_policy))
        .unwrap();

    let config = Config::builder()
        .appender(
            Appender::builder()
                .filter(Box::new(ThresholdFilter::new(LevelFilter::Debug)))
                .build("file", Box::new(file))
        )
        .build(
            Root::builder().appender("file").build(lvl)
        )
        .unwrap();

    log4rs::init_config(config).unwrap();
}


// ////////////////////////////////////////////////////////////////////////////////////
//
// 下面的5个 'qrust_xxx()' 函数是供QT调用的接口函数。
// 除了 'qrust_init()' 函数可用来进行初始化工作外，其他4个函数请不要修改。
//
// The following five 'qrust_xxx()' functions are interface functions for QT to call.
// Do not modify the other four functions except the 'qrust_init()' function which can be 
// used for initialization.
//
// ////////////////////////////////////////////////////////////////////////////////////

use libc::{c_uchar, c_int};
use std::ffi::{CString, c_char};

/// 初始调用（例如初始化日志）
/// Initial calls (e.g. initializing logs)
#[no_mangle]
pub unsafe extern "C" fn qrust_init()
{
    //rust日志系统可以自定义，这里使用log4rs是个示例。
    //同样道理，这里通过环境变量获得日志输出路径也是示例目的，您可以改为配置文件，或其他方式。
    //The rust logging system can be customized, here using log4rs is an example.
    //In the same way, obtaining the log output path through the environment variable is also 
    //the purpose of the example, you can change to the configuration file, or other methods.
    match env::var("LOG_PATH")
    {
        Err(_) =>
        {
            return;
        }
        Ok(log_path) =>
        {
            log_init(LevelFilter::Debug, &log_path);
        }
    }  
}

/// 函数调用
/// Function call
#[no_mangle]
pub unsafe extern "C" fn qrust_call(in_ptr: *const c_uchar, size: c_int) -> Ret
{
    let pack: &[u8] = std::slice::from_raw_parts(in_ptr, size as usize);
    let mut p: usize = 0; 
    // debug!("input pack: {:?}, len: {}, p: {}", pack, pack.len(), p);

    //解析函数名  Parsing function name
    let fun_name = match to_fun_name(pack, &mut p) 
    {
        None =>
        {
            return Ret{
                ptr:    std::ptr::null_mut(), 
                len:    0, 
                code:   serde_qrust::tag::FUN_NAME_ONT_EXIST};
        }
        Some(fun_name) =>
        {
            fun_name
        }
    };
    debug!("fun_name: {}", fun_name);

    let args_pack = &pack[p..];
    //解析函数参数  Parsing function parameter
    let args: Vec<&[u8]> = match to_fun_args(args_pack)
    {
        None =>
        {
            let ret = Ret{
                ptr:    std::ptr::null_mut(), 
                len:    0,
                code:   serde_qrust::tag::FUN_ARGS_ERROR,
            };
            return ret;
        }
        Some(vec) => vec,
    };

    //调用函数  Calling function
    match invoke(fun_name, args)
    {
        Ok(Some(v)) =>
        {
            let ret = pack_ret(v);
            ret
        }
        Ok(None) =>
        {
            let ret = Ret{
                ptr:    std::ptr::null_mut(), 
                len:    0,
                code:   tag::RET_VOID,
            };
            ret
        }
        Err(e) =>
        {
            error!("invoke error: {}", e);
            let ret = Ret{
                ptr:    std::ptr::null_mut(), 
                len:    0,
                code:   serde_qrust::tag::INVOKE_FAILURE,
            };
            ret
        }
    }
}

/// 释放返回值结构
/// Release the return value structure
#[no_mangle]
pub unsafe extern "C" fn qrust_free_ret(ret: Ret)
{
    let ret_ptr = unsafe { std::slice::from_raw_parts_mut(ret.ptr, ret.len) };
    let _ = Box::from_raw(ret_ptr);
}

/// 释放调用pack::from_str_ptr()时，在函数内部创建的CString
/// Release the CString created inside the function when pack::from_str_ptr() is called
#[no_mangle]
pub unsafe extern "C" fn qrust_free_str(ptr: *const c_char)
{
    debug!("qrust_free_str(): {:p}", ptr);
    let _: CString = CString::from_raw(ptr as *mut _);
}

/// 释放调用pack::from_bytes_ptr_clone()时，在函数内部克隆的Vec<u8>
/// Release Vec<u8> that was cloned inside the function when pack::from_bytes_ptr_clone() was called.
#[no_mangle]
pub unsafe extern "C" fn qrust_free_bytes(ptr: *const u8, len: c_int)
{
    debug!("qrust_free_bytes(): {:p}, len: {}", ptr, len);
    let _: Vec<u8> = unsafe{ Vec::from_raw_parts(ptr as *mut u8, len as usize, len as usize) };
}


// ////////////////////////////////////////////////////////////////////////////////////
//
// 内部函数， 请不要修改。
// Internal function, please do not modify.
//
// ////////////////////////////////////////////////////////////////////////////////////

/// 解析函数名
/// Parsing function name
fn to_fun_name<'a>(pack: &'a [u8], p: &mut usize) -> Option<&'a str>
{
    // debug!("pack: {:?}, len: {}, p: {}", pack, pack.len(), p);
    //parse fun name-----------
    if pack.len() < 3  
    {
        error!("[qrust] The function name is too short");
        return None;  
    }
    // debug!("fun tag: {}", pack[*p]);
    if pack[*p] != tag::FUN_NAME    
    {
        error!("[qrust] The function name tag error");
        return None; 
    }
    let name_len = pack[1];
    if (2+name_len) as usize > pack.len() 
    {
        error!("[qrust] Function name length definition is out of bounds");
        return None; 
    }
    let fun_name = match std::str::from_utf8(&pack[2..(2+name_len) as usize])
    {
        Ok(name) => name,
        Err(e) => 
        {
            error!("[qrust] The function name has illegal characters: {}", e);
            return None; 
        }
    };
    *p += 2 + name_len as usize;

    Some(fun_name)
}

/// 解析函数参数  Parsing function parameter
/// pack: 从FUN_ARGS开始的包  Packages starting with FUN_ARGS
/// count: 参数数量  Number of parameters
fn to_fun_args(pack: &[u8]) -> Option<Vec<&[u8]>>
{
    // debug!("Args pack: {:?}", pack);

    if pack.len() < 6
    {
        error!("Function args formatting error");
        return None;
    }
    if pack[0] != tag::FUN_ARGS
    {
        error!("Function args formatting error");
        return None;
    }

    let count = pack[1] as usize;
    // debug!("Args count: {}, pack len: {}", count, pack.len());

    let mut vec: Vec<&[u8]> = Vec::new();
    let pack = &pack[6..];

    if pack.len() == 0
    {
        return Some(vec);  //无参数  no parameters
    }

    let mut p = 0;
    while p < pack.len() 
    {
        match pack[p]
        {
            tag::BOOL | tag::I8 | tag::U8 => 
            {
                if p + 2 > pack.len() {break};
                let item = &pack[p..p+2];
                // debug!("item: {:?}", item);
                vec.push(item);
                p += 2;
            }
            tag::I16 | tag::U16 => 
            {
                if p + 3 > pack.len() {break};
                let item = &pack[p..p+3];
                // debug!("item: {:?}", item);
                vec.push(item);
                p += 3;
            }
            tag::I32 | tag::U32 | tag:: F32 => 
            {
                if p + 5 > pack.len() {break};
                let item = &pack[p..p+5];
                // debug!("item: {:?}", item);
                vec.push(item);
                p += 5;
            }
            tag::I64 | tag::U64 | tag:: F64 => 
            {
                if p + 9 > pack.len() {break};
                let item = &pack[p..p+9];
                // debug!("item: {:?}", item);
                vec.push(item);
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
                let item = &pack[p..p + 5 + len];
                vec.push(item);
                p = p + 5 + len;
            }
            tag::STRUCT =>
            {
                if p + 2 > pack.len() {break};
                let name_len = pack[p+1] as usize; //struct名称长度  The length of the struct name
                if p + 2 + name_len + 5 > pack.len() {break};
                let mut byte4: [u8; 4] = [0; 4];
                byte4[0] = pack[p + 2 + name_len + 1];
                byte4[1] = pack[p + 2 + name_len + 2];
                byte4[2] = pack[p + 2 + name_len + 3];
                byte4[3] = pack[p + 2 + name_len + 4];
                let body_len = u32::from_be_bytes(byte4) as usize;
                if p + 2 + name_len + 5 + body_len > pack.len() {break};
                let item = &pack[p..p + 2 + name_len + 5 + body_len];
                // debug!("item: {:?}", item);
                vec.push(item);
                p = p + 2 + name_len + 5 + body_len;
            }
            _ =>
            {
                error!("Unknown arg, tag: {}", pack[p]);
                break;
            }
        }
    }
    
    if count != vec.len()
    {
        error!("Args count error");
        None
    }
    else 
    {
        vec.reverse(); //反转  reverse
        Some(vec)
    } 
}

/// 返回值结构
/// Return value structure
#[repr(C)]
pub struct Ret 
{
    pub ptr:    *mut u8,   //返回值Vec<u8>的指针  The pointer to the return value(Vec<u8>)
    pub len:    usize,     //返回值Vec<u8>的长度  The length of the returned value
    pub code:   u8,        //0正常, 非0失败（参见tag）  0 normal, non-0 failed (see tag)
}
/// Vec<u8>包 转 返回值结构
/// Vec<u8> Packet to return value structure
fn pack_ret(ret: Vec<u8>) -> Ret
{
    let mut buf = ret.into_boxed_slice();
    let ptr = buf.as_mut_ptr();
    let len = buf.len();
    let code = 0; //正常  normal

    std::mem::forget(buf);
    Ret { ptr, len, code }
}