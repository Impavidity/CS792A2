
struct thrift_dir_entry {
    1: i64 dir_id;
    2: i64 dir_type;
    3: string dir_name;
}

struct thrift_readdir_reply {
    1: i32 ret;
    2: list<thrift_dir_entry> dir_entries;
}

struct thrift_file_info {
    1: i32 flags;
    2: i64 fh;
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

service NFS {
    thrift_readdir_reply nfs_readdir(1:string tpath);
    i32 nfs_mkdir(1:string tpath, 2:i32 mode);
    i32 nfs_rmdir(1:string tpath);
    thrift_getattr_reply nfs_getattr(1:string tpath)
    void ping();
}