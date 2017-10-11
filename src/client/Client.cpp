//
// Created by shipeng on 17-10-8.
//

#include "RPCClient.h"

static RPCClient rpcClient("localhost", 9090);

static int nfs_create(const char *path, mode_t mode, struct fuse_file_info* fi) {
  int fh;
  rpcClient.nfs_create(path, mode, fi);
}

static int nfs_readdir(const char* path, void * buf, fuse_fill_dir_t filler, off_t offset,
                       struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
  int ret;
  ret = rpcClient.nfs_readdir(path, buf, filler, offset, fi, flags);
  return ret;
}

static int nfs_mkdir(const char* path, mode_t mode) {
  int ret;
  std::cout << "In Mkdir " << path <<std::endl;
  ret = rpcClient.nfs_mkdir(path, mode);
  return ret;
}

static int nfs_rmdir(const char* path) {
  int ret;
  std::cout << "In RMDIR" << path << std::endl;
  ret = rpcClient.nfs_rmdir(path);
  return ret;
}

static int nfs_getattr(const char* path, struct stat* stbuf,
                        struct fuse_file_info *fi) {
  int ret;
  ret = rpcClient.nfs_getattr(path, stbuf, fi);
  return ret;
}

//static int nfs_read()

static void *nfs_init(struct fuse_conn_info *conn,
                      struct fuse_config *cfg)
{
  (void) conn;
  cfg->kernel_cache = 1;
  return NULL;
}

struct nfs_fuse_operations : fuse_operations {
  nfs_fuse_operations () {
    init = nfs_init;
    getattr = nfs_getattr;
    readdir = nfs_readdir;
    mkdir = nfs_mkdir;
    rmdir = nfs_rmdir;
    //.create = nfs_create,
  }
};

static struct nfs_fuse_operations nfs_oper;


static struct options {
  const char *filename;
  const char *contents;
  int show_help;
} options;




static int hello_getattr(const char *path, struct stat *stbuf,
                         struct fuse_file_info *fi)
{
  (void) fi;
  int res = 0;

  memset(stbuf, 0, sizeof(struct stat));
  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
  } else if (strcmp(path+1, options.filename) == 0) {
    stbuf->st_mode = S_IFREG | 0444;
    stbuf->st_nlink = 1;
    stbuf->st_size = strlen(options.contents);
  } else
    res = -ENOENT;

  return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi,
                         enum fuse_readdir_flags flags)
{
  /*
  (void) offset;
  (void) fi;
  (void) flags;

  if (strcmp(path, "/") != 0)
    return -ENOENT;
  */
  filler(buf, ".", NULL, 0, FUSE_FILL_DIR_PLUS);
  filler(buf, "..", NULL, 0, FUSE_FILL_DIR_PLUS);
  filler(buf, options.filename, NULL, 0, FUSE_FILL_DIR_PLUS);

  std::cout << "I am in hello readdir" << std::endl;
  return 0;
}

static int hello_open(const char *path, struct fuse_file_info *fi)
{
  if (strcmp(path+1, options.filename) != 0)
    return -ENOENT;

  if ((fi->flags & O_ACCMODE) != O_RDONLY)
    return -EACCES;

  return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
  size_t len;
  (void) fi;
  if(strcmp(path+1, options.filename) != 0)
    return -ENOENT;

  len = strlen(options.contents);
  if (offset < len) {
    if (offset + size > len)
      size = len - offset;
    memcpy(buf, options.contents + offset, size);
  } else
    size = 0;

  return size;
}

static struct hello_fuse_operations : fuse_operations  {
  hello_fuse_operations() {
    init = nfs_init;
    getattr = hello_getattr;
    //readdir = hello_readdir;
    //open = hello_open;
    //read = hello_read;
    mkdir = nfs_mkdir;
  }
} hello_oper;



/*
int main(int argc, char *argv[])
{

  options.filename = strdup("hello");
  options.contents = strdup("Hello World!\n");

  return fuse_main(argc, argv, &hello_oper, NULL);
}
*/


int main(int argc, char *argv[]) {
  std::cout << "Client Start ..." << std::endl;
  umask(0);
  return fuse_main(argc, argv, &nfs_oper, NULL);
}
