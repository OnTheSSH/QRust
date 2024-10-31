# QRust

QRust is an open source component related to mixed programming in both Qt and Rust languages, and is a support technology for Qt calling Rust functions.

QRust comes from the tool software OnTheSSH, which is jointly built by Qt and Rust. Rust implements the underlying protocol of SSH communication, Qt builds the program interface, and the technical requirements of Qt calling Rust give rise to QRust.

## An example of using QRust:
Rust:
```
fn invoke(fun_name: &str, mut args: Vec<&[u8]>) -> Result<Option<Vec<u8>>>
{
    match fun_name
    {
        ……
        "foo2" =>
        {
            let a1 = de::from_pack(args.pop().unwrap())?;  //Deserialization gets parameter 1
            let a2 = de::from_pack(args.pop().unwrap())?;    //Deserialization gets parameter 2
            let a3 = de::from_pack(args.pop().unwrap())?;  //Deserialization gets parameter 3
 
            let ret = api::foo2(a1, a2, a3);    //Call function foo2 and get the return value
            let pack = ser::to_pack(&ret)?;     //Serialized return value
            Ok(Some(pack))
        }
        ……
    }
}
```
In the above code, by matching the string “foo2” to the function api::foo2(), deserialize the three parameters and serialize the return value of the function.
Qt:
```
Rust rust("foo2");  //Declare the Rust function foo2 to call
 
//Parameter 1
QList<qint32> a1 = {100};
QByteArray ba1 = QRust_Ser::pack_list_i32(&a1);  //serialize
//Parameter 2
QHash<qint32, QString> a2 = {{1, "abcde"}};
QByteArray ba2 = QRust_Ser::pack_hi_str(&a2);  //serialize
//Parameter 3
QHash<QString, QString> a3 = {{"a", "12345中文"}};
QByteArray ba3 = QRust_Ser::pack_hs_str(&a3);  //serialize
 
rust.call(ba1, ba2, ba3);  //Call the function and pass arguments
 
QHash<QString, QString> ret;  //Declared return value
QRust_De::upack_hs_str(rust.pop(), &ret);  //Deserialization gets the return value
```
In the above code, declare the Rust side function foo2 to call, serialize and pass three parameters, and then deserialize the function call to get the return value. In the example, transformations of three complex data types are implemented:
Qt: QList<qint32>, QHash<qint32, QString>, QHash<QString, QString>
Rust Vec<i32>, HashMap<i32, String>, HashMap<String, String>

## More information:
1. [QRust (1) Introduction](https://onthessh.com/articles/qrust-1-introduction/)
2. [QRust (2) Data Type](https://onthessh.com/articles/qrust-2-data-type/)
3. [QRust (3) Framework](https://onthessh.com/articles/qrust-3-framework/)
4. [QRust (4) Example](https://onthessh.com/articles/qrust-4-example/)
