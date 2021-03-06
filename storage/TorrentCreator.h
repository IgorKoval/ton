#pragma once
#include "Torrent.h"

#include "td/utils/optional.h"
#include "td/utils/SharedSlice.h"
#include "td/db/utils/BlobView.h"

namespace ton {
class Torrent::Creator {
 public:
  struct Options {
    td::uint32 piece_size{128 * 768};

    // override default dir_name
    // should't be used in a usual workflow
    td::optional<std::string> dir_name;

    std::string description;
  };

  // If path is a file create a torrent with one file in it.
  // If path is a directory, create a torrent with whole directory.
  static td::Result<Torrent> create_from_path(Options options, td::CSlice path);
  struct Blob {
    td::Slice name;
    td::Slice data;
  };
  static td::Result<Torrent> create_from_blobs(Options options, td::Span<Blob> blobs);

  // This api is mostly for internal usage. One should prefer static methods
  explicit Creator(Options options) : options_(options) {
  }
  TD_WARN_UNUSED_RESULT td::Status add_blob(td::Slice name, td::Slice blob);
  TD_WARN_UNUSED_RESULT td::Status add_blob(td::Slice name, td::BlobView blob);
  TD_WARN_UNUSED_RESULT td::Status add_file(td::Slice name, td::CSlice path);
  td::Result<Torrent> finalize();

 private:
  td::Status init();

  Options options_;
  struct File {
    std::string name;
    td::BlobView data;
  };
  std::vector<File> files_;
};
}  // namespace ton
