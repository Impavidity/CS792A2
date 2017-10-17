
struct thrift_dir_entry {
    1: i64 dir_id;
    2: i64 dir_type;
    3: string dir_name;
}

struct thrift_readdir_reply {
    1: i32 ret;
    2: list<thrift_dir_entry> dir_entries;
}

struct thrift_timespec {
    1: i64 tv_sec;
    2: i64 tv_nsec;
}

struct thrift_stat {
    1: i64 st_dev;
    2: i64 st_ino;
    3: i64 st_nlink;
    4: i32 st_mode;
    5: i32 st_uid;
    6: i32 st_gid;
    7: i32 __pad0;
    8: i64 st_rdev;
    9: i64 st_size;
    10:i64 st_blksize;
    11:i64 st_blocks;
    12:thrift_timespec st_atim;
    13:thrift_timespec st_mtim;
    14:thrift_timespec st_ctim;
    15:i64 __glibc_reserved0;
    16:i64 __glibc_reserved1;
    17:i64 __glibc_reserved2;
}

struct thrift_getattr_reply {
    1: i32 ret;
    2: thrift_stat tstbuf;
}

struct thrift_file_handler {
    1: i32 system_id;
    2: i64 inode;
    3: i64 generation_number;
}

struct thrift_file_handler_reply {
    1: i32 ret;
    2: thrift_file_handler fh;
}

struct thrift_read_reply {
    1: i32 ret;
    2: binary buf;
}

service NFS {
    thrift_file_handler root();
    thrift_file_handler mount(1:string path);
    thrift_file_handler lookup(1:thrift_file_handler fh, 2:string name);
    thrift_getattr_reply getattr(1:thrift_file_handler fh);
    thrift_readdir_reply readdir(1:thrift_file_handler fh);
    thrift_file_handler_reply mkdir(1:thrift_file_handler fh, 2:string name);
    i32 rmdir(1:thrift_file_handler fh);
    i32 unlink(1:thrift_file_handler fh);
    i32 rename(1:thrift_file_handler fh, 2:string toname);
    thrift_file_handler_reply create(1:thrift_file_handler fh, 2:string name);
    thrift_read_reply read(1:thrift_file_handler fh, 2:i64 size, 3:i64 offset);
    i32 write(1:thrift_file_handler fh, 2:binary buf, 3:i64 size, 4:i64 offset);
    i32 fsync(1:thrift_file_handler fh);
}
