//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_file.hpp"
#include "my_globals.hpp"
#include "my_log.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_ramdisk.hpp"
#include "my_string.hpp"

#include <cmath>
#include <cstdarg>
#include <cstring>
#include <ctime>
#include <print>
#include <sys/stat.h>
#include <unistd.h>

static auto file_io_read_if_exists(const char *filename, int *out_len) -> uint8_t *;
static auto file_io_read(const char *filename, int *len) -> uint8_t *;

auto file_load(const char *filename, int *outlen) -> uint8_t *
{
  TRACE();

  uint8_t *out          = nullptr; // NOLINT
  char    *alt_filename = nullptr;

  alt_filename = nullptr;

  /*
   * If the file is on disk and is newer than the program, use that in
   * preference.
   */
  if (static_cast< bool >(file_exists(filename))) {
    if (strstr(filename, "data/") != nullptr) {
      if (static_cast< bool >(file_exists_and_is_newer_than(filename, g_exec_full_path_and_name))) {
        out = file_io_read_if_exists(filename, outlen);
        if (out != nullptr) {
          file_log("Read file %s (newer than exec)", filename);
          return out;
        }
      }

      if (static_cast< bool >(file_exists_and_is_newer_than(filename, ".o/file.o"))) {
        out = file_io_read_if_exists(filename, outlen);
        if (out != nullptr) {
          file_log("Read file %s (newer than build)", filename);
          return out;
        }
      }

      if (static_cast< bool >(file_exists_and_is_newer_than(filename, "src/.o/file.o"))) {
        out = file_io_read_if_exists(filename, outlen);
        if (out != nullptr) {
          file_log("Read file %s (newer than src build)", filename);
          return out;
        }
      }
    } else {
      out = file_io_read_if_exists(filename, outlen);
      if (out != nullptr) {
        file_log("Read file %s (exists locally)", filename);
        return out;
      }
    }
  }

  if (g_exec_dir != nullptr) {
    alt_filename = strprepend(filename, g_exec_dir);

    if (static_cast< bool >(file_exists(alt_filename))) {
      if (static_cast< bool >(file_exists_and_is_newer_than(alt_filename, g_exec_full_path_and_name))) {
        out = file_io_read_if_exists(alt_filename, outlen);
        if (out != nullptr) {
          MYFREE(alt_filename);
          alt_filename = nullptr;

          return out;
        }
      }

      if (static_cast< bool >(file_exists_and_is_newer_than(alt_filename, ".o/file.o"))) {
        out = file_io_read_if_exists(alt_filename, outlen);
        if (out != nullptr) {
          MYFREE(alt_filename);
          alt_filename = nullptr;

          return out;
        }
      }

      if (static_cast< bool >(file_exists_and_is_newer_than(alt_filename, "src/.o/file.o"))) {
        out = file_io_read_if_exists(alt_filename, outlen);
        if (out != nullptr) {
          MYFREE(alt_filename);
          alt_filename = nullptr;

          return out;
        }
      }
    }
  }

  auto *r = ramdisk_load(filename, outlen);
  if (r != nullptr) {
    file_log("Read (ramdisk) %s, %d Mb, %d bytes", filename, *outlen / (1024 * 1024), *outlen);

    if (alt_filename != nullptr) {
      MYFREE(alt_filename);
      alt_filename = nullptr;
    }

    return r;
  }

  /*
   * Fallback to the disk.
   */
  out = file_io_read_if_exists(filename, outlen);
  if (out != nullptr) {
    if (alt_filename != nullptr) {
      MYFREE(alt_filename);
      alt_filename = nullptr;
    }

    return out;
  }

  out = file_io_read_if_exists(alt_filename, outlen);
  if (out != nullptr) {
    if (alt_filename != nullptr) {
      MYFREE(alt_filename);
      alt_filename = nullptr;
    }

    return out;
  }

  if (alt_filename != nullptr) {
    MYFREE(alt_filename);
    alt_filename = nullptr;
  }

  {
    std::string const base_alt_filename = mybasename(filename, "strip dir");

    out = file_io_read_if_exists(base_alt_filename.c_str(), outlen);
    if (out != nullptr) {
      return out;
    }
  }

  /*
   * Fail. Caller should whinge.
  char *popup_str = dynprintf("Filename was not found on ramdisk or "
                "on the local disk, %s", filename);

  MYFREE(popup_str);
   */
  log("file not found \"%s\"", filename);

  return nullptr;
}

auto file_io_read(const char *filename, int *out_len) -> uint8_t *
{
  TRACE();

  uint8_t *buffer = nullptr;
  FILE    *file   = nullptr;
  int      len    = 0;

  file = fopen(filename, "rb");
  if (file == nullptr) {
    std::println(MY_STDERR, "failed to open file \"{}\" for reading: {}", filename, strerror(errno));
    return nullptr;
  }

  /*
   * Get the file size.
   */
  if (fseek(file, 0, SEEK_END) != 0) {
    std::println(MY_STDERR, "failed to seek end of file \"{}\": {}", filename, strerror(errno));
    fclose(file);
    return nullptr;
  }

  len = static_cast< uint32_t >(ftell(file));
  if (len == -1) {
    std::println(MY_STDERR, "failed to get size of file \"{}\": {}", filename, strerror(errno));
    fclose(file);
    return nullptr;
  }

  if (fseek(file, 0, SEEK_SET) != 0) {
    std::println(MY_STDERR, "failed to seek begin of file \"{}\": {}", filename, strerror(errno));
    fclose(file);
    return nullptr;
  }

  buffer = static_cast< uint8_t * >(MYMALLOC(len + SIZEOF((char) '\0'), "file read"));
  if (buffer == nullptr) {
    std::println(MY_STDERR, "failed to alloc mem for file \"{}\": {}", filename, strerror(errno));
    fclose(file);
    return nullptr;
  }

  if (! static_cast< bool >(fread(buffer, len, 1, file))) {
    std::println(MY_STDERR, "failed to read file \"{}\": {}", filename, strerror(errno));
    fclose(file);
    return nullptr;
  }

  if (out_len != nullptr) {
    *out_len = len;
  }

  file_log("Read %s, %d Mb, %d bytes", filename, len / (1024 * 1024), len);

  fclose(file);

  return buffer;
}

auto file_write(const char *filename, uint8_t *buffer, int len) -> int
{
  TRACE();

  FILE   *file = nullptr;
  uint8_t rc   = 0;

  file = fopen(filename, "w");
  if (file == nullptr) {
    ERR("failed to open file \"%s\" for writing: %s\n", filename, strerror(errno));
    return -1;
  }

  rc = fwrite(buffer, len, 1, file);

  file_log("Saved %s, %d bytes", filename, len);

  /*
   * Check written one object.
   */
  if (! static_cast< bool >(rc)) {
    ERR("failed to write file \"%s\": %s\n", filename, strerror(errno));
    fclose(file);
    return -1;
  }

  if (ferror(file) != 0) {
    ERR("Error writing to write file \"%s\": %s\n", filename, strerror(errno));
    fclose(file);
    return -1;
  }

  fclose(file);
  return 0;
}

/*
 * Does the requested file exist?
 */
auto file_exists(const char *filename) -> uint8_t
{
  TRACE();

  struct stat buf {};

  if (filename == nullptr) {
    return 0;
  }

  if (stat(filename, &buf) >= 0) {
    if (S_ISDIR(buf.st_mode)) {
      return 0;
    }

    return 1;
  }
  return 0;
}

auto file_io_read_if_exists(const char *filename, int *out_len) -> uint8_t *
{
  TRACE();

#if 0
  uint8_t *ret;
  char *mz_filename;

  if (!filename) {
    return 0;
  }

  if (strstr(filename, ".mz")) {
    ret = mzip_file_read(filename, out_len);
    return ret;
  }

  mz_filename = strappend(filename, ".mz");
  if (!mz_filename) {
    return 0;
  }

  if (file_exists(mz_filename)) {
    ret = mzip_file_read(mz_filename, out_len);
    MYFREE(mz_filename);
    return ret;
  }

  MYFREE(mz_filename);
#endif

  if (static_cast< bool >(file_exists(filename))) {
    return file_io_read(filename, out_len);
  }

  return nullptr;
}

/*
 * How large is the file?
 */
auto file_size(const char *filename) -> int
{
  TRACE();

  struct stat buf {};

  if (stat(filename, &buf) >= 0) {
    return static_cast< int >(buf.st_size);
  }

  return -1;
}

/*
 * Does the requested file exist?
 */
auto file_non_zero_size_exists(const char *filename) -> uint8_t
{
  TRACE();

  if (! static_cast< bool >(file_exists(filename))) {
    return 0;
  }

  if (file_size(filename) == 0) {
    return 0;
  }

  return 1;
}

/*
 * Remove the file if it exists.
 */
auto file_unlink(const char *filename) -> uint8_t
{
  TRACE();

  if (! static_cast< bool >(file_exists(filename))) {
    return 0;
  }

  unlink(filename);

  return 1;
}

/*
 * Modification time.
 */
auto file_age(const char *filename) -> double
{
  TRACE();

  struct stat buf {};

  if (stat(filename, &buf) < 0) {
    return -1;
  }

  time_t const now = time(nullptr);

  return difftime(now, buf.st_mtime);
}

/*
 * If the first filename newer than the second ?
 */
auto file_exists_and_is_newer_than(const char *filename1, const char *filename2) -> uint8_t
{
  TRACE();

  struct stat buf1 {};
  struct stat buf2 {};
  double      delta = 0;

  if (filename1 == nullptr) {
    CROAK("expected two filenames");
  }

  if (filename2 == nullptr) {
    CROAK("expected two filenames");
  }

  // fprintf(stdout, "%s/%s\n", filename1, filename2);

  if (stat(filename1, &buf1) < 0) {
    return 0U;
  }

  if (stat(filename2, &buf2) < 0) {
    return 0U;
  }

  delta = difftime(buf1.st_mtime, buf2.st_mtime);

  return static_cast< uint8_t >(delta > 0);
}

void file_log(const char *fmt, ...)
{
  va_list args = {};

  va_start(args, fmt);
  log_(fmt, args);
  va_end(args);
}

void file_dbg(const char *fmt, ...)
{
  va_list args = {};

  IF_NODEBUG2 { return; }

  va_start(args, fmt);
  log_(fmt, args);
  va_end(args);
}
