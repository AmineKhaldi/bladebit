#pragma once
#include "Platform.h"
#include "IStream.h"

enum class FileAccess : uint16
{
    None  = 0,
    Read  = 1 << 0,
    Write = 1 << 1,
    ReadWrite = Read | Write
};
ImplementFlagOps( FileAccess );

enum class FileMode : uint16
{
    Open          = 0     // Open existing (fails if it does not exist).
    ,Create       = 1     // Create new, or if it is existing, open and truncate.
    ,OpenOrCreate = 2     // Open existing or create new.
};

enum class FileFlags : uint32
{
    None        = 0,
    NoBuffering = 1 << 0,
    LargeFile   = 1 << 1,
};
ImplementFlagOps( FileFlags );


class FileStream : public IStream
{
public:
    inline FileStream() {}
    inline ~FileStream()
    {
        Close();
    }

    // Duplicate a file
    // FileStream( const FileStream& src );

    // void WriteAsync( byte* buffer, size_t size );

    static bool Open( const char* path, FileStream& file, FileMode mode, FileAccess access, FileFlags flags = FileFlags::None );
    bool Open( const char* path, FileMode mode, FileAccess access, FileFlags flags = FileFlags::None );

    ssize_t Read( void* buffer, size_t size ) override;
    ssize_t Write( const void* buffer, size_t size ) override;

    bool Reserve( ssize_t size );

    bool Seek( int64 offset, SeekOrigin origin ) override;

    bool Flush() override;

    inline size_t BlockSize() const override
    {
        return _blockSize;
    }

    ssize_t Size() override;

    void Close();

    bool IsOpen() const;
    static bool Exists( const char* path );

    inline int GetError() override
    {
        int err = _error;
        _error = 0;

        return err;
    }

    inline FileAccess GetFileAccess() const { return _access; }

    inline FileFlags GetFlags() const { return _flags; }

    inline intptr_t Id() { return (intptr_t)_fd; }

private:
    inline bool HasValidFD() const
    {
        #if PLATFORM_IS_UNIX
            return _fd != -1;
        #else
            return _fd != INVALID_HANDLE_VALUE;
        #endif
    }

private:
    size_t     _writePosition = 0;
    size_t     _readPosition  = 0;
    FileAccess _access        = FileAccess::None;
    FileFlags  _flags         = FileFlags::None;
    int        _error         = 0;
    size_t     _blockSize     = 0;        // for O_DIRECT/FILE_FLAG_NO_BUFFERING

    #if PLATFORM_IS_UNIX
        int    _fd            = -1;
    #elif PLATFORM_IS_WINDOWS
        HANDLE _fd            = INVALID_HANDLE_VALUE;
    #endif
};