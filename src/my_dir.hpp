//
// Copyright goblinhack@gmail.com
//

#ifndef MY_FILENODE_HPP
#define MY_FILENODE_HPP

[[nodiscard]] auto dir_exists(const std::string &) -> bool;
[[nodiscard]] auto dir_dot() -> char *;
[[nodiscard]] auto dir_dotdot(char *s) -> char *;
[[nodiscard]] auto dospath2unix(char *in) -> char *;

#endif
