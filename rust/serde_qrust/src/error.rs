use serde::{de, ser};
use std::fmt::{self, Display};

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    // 通过 `ser::Error` 和 `de::Error` traits 可以由数据结构创建的一个或多个变体。
    // 例如，对于 Mutex<T> 的 Serialize 实现可能会返回一个错误，因为 mutex 被 poisoned，
    // 或者对于结构体的 Deserialize 实现可能会因为一个必需的字段丢失而返回错误。
    Message(String),

    // 通过 Serializer 和 Deserializer 直接创建的一个或多个变体，无需经过`ser::Error` 和 `de::Error`。
    // 这些特定于格式，在这种情况下是 JSON。
    Eof,
    Syntax,
    TrailingCharacters,

    ExpectedFun,

    ExpectedBool,
    ExpectedI8,
    ExpectedI16,
    ExpectedI32,
    ExpectedI64,
    ExpectedU8,
    ExpectedU16,
    ExpectedU32,
    ExpectedU64,
    ExpectedF32,
    ExpectedF64,
    ExpectedString,
    ExpectedBytes,

    ExpectedList,
    ExpectedListEof,
    ExpectedMap,
    ExpectedMapEof,
    ExpectedMapKeyType,
    ExpectedStruct,
    ExpectedStructEof,
    ExpectedArgs,
    ExpectedArgsEof,
}

impl ser::Error for Error {
    fn custom<T: Display>(msg: T) -> Self {
        Error::Message(msg.to_string())
    }
}

impl de::Error for Error {
    fn custom<T: Display>(msg: T) -> Self {
        Error::Message(msg.to_string())
    }
}

impl Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Error::Message(msg)=> write!(f, "{}", msg),
            Error::Eof                  => f.write_str("Eof"),
            Error::Syntax               => f.write_str("Syntax"),
            Error::TrailingCharacters   => f.write_str("TrailingCharacters"),
            Error::ExpectedFun          => f.write_str("ExpectedFun"),
            Error::ExpectedBool         => f.write_str("ExpectedBool"),
            Error::ExpectedI8           => f.write_str("ExpectedI8"),
            Error::ExpectedI16          => f.write_str("ExpectedI16"),
            Error::ExpectedI32          => f.write_str("ExpectedI32"),
            Error::ExpectedI64          => f.write_str("ExpectedI64"),
            Error::ExpectedU8           => f.write_str("ExpectedBool"),
            Error::ExpectedU16          => f.write_str("ExpectedU16"),
            Error::ExpectedU32          => f.write_str("ExpectedU32"),
            Error::ExpectedU64          => f.write_str("ExpectedU64"),
            Error::ExpectedF32          => f.write_str("ExpectedF32"),
            Error::ExpectedF64          => f.write_str("ExpectedF64"),
            Error::ExpectedString       => f.write_str("ExpectedString"),
            Error::ExpectedBytes        => f.write_str("ExpectedBytes"),
            Error::ExpectedList         => f.write_str("ExpectedList"),
            Error::ExpectedListEof      => f.write_str("ExpectedListEof"),
            Error::ExpectedMap          => f.write_str("ExpectedMap"),
            Error::ExpectedMapEof       => f.write_str("ExpectedMapEof"),
            Error::ExpectedMapKeyType   => f.write_str("ExpectedMapKeyType"),
            Error::ExpectedStruct       => f.write_str("ExpectedStruct"),
            Error::ExpectedStructEof    => f.write_str("ExpectedStructEof"),
            Error::ExpectedArgs         => f.write_str("ExpectedArgs"),
            Error::ExpectedArgsEof      => f.write_str("ExpectedArgsEof"),
        }
    }
}

impl std::error::Error for Error {}
