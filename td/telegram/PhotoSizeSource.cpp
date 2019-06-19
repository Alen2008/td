//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2019
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "td/telegram/PhotoSizeSource.h"

#include "td/telegram/ChannelId.h"
#include "td/telegram/ChatId.h"
#include "td/telegram/UserId.h"

#include "td/utils/common.h"

namespace td {

tl_object_ptr<telegram_api::InputPeer> PhotoSizeSource::DialogPhoto::get_input_peer() const {
  switch (dialog_id.get_type()) {
    case DialogType::User: {
      UserId user_id = dialog_id.get_user_id();
      return make_tl_object<telegram_api::inputPeerUser>(user_id.get(), dialog_access_hash);
    }
    case DialogType::Chat: {
      ChatId chat_id = dialog_id.get_chat_id();
      return make_tl_object<telegram_api::inputPeerChat>(chat_id.get());
    }
    case DialogType::Channel: {
      ChannelId channel_id = dialog_id.get_channel_id();
      return make_tl_object<telegram_api::inputPeerChannel>(channel_id.get(), dialog_access_hash);
    }
    case DialogType::SecretChat:
      return nullptr;
    case DialogType::None:
      return make_tl_object<telegram_api::inputPeerEmpty>();
    default:
      UNREACHABLE();
      return nullptr;
  }
}

FileType PhotoSizeSource::get_file_type() const {
  switch (get_type()) {
    case PhotoSizeSource::Type::Thumbnail:
      return thumbnail().file_type;
    case PhotoSizeSource::Type::DialogPhoto:
      return FileType::ProfilePhoto;
    case PhotoSizeSource::Type::StickerSetThumbnail:
      return FileType::Thumbnail;
    case PhotoSizeSource::Type::Empty:
    default:
      UNREACHABLE();
      return FileType::Thumbnail;
  }
}

bool operator==(const PhotoSizeSource &lhs, const PhotoSizeSource &rhs) {
  if (lhs.get_type() != rhs.get_type()) {
    return false;
  }
  switch (lhs.get_type()) {
    case PhotoSizeSource::Type::Thumbnail:
      return lhs.thumbnail().file_type == rhs.thumbnail().file_type &&
             lhs.thumbnail().thumbnail_type == rhs.thumbnail().thumbnail_type;
    case PhotoSizeSource::Type::DialogPhoto:
      return lhs.dialog_photo().dialog_id == rhs.dialog_photo().dialog_id &&
             lhs.dialog_photo().dialog_access_hash == rhs.dialog_photo().dialog_access_hash &&
             lhs.dialog_photo().is_big == rhs.dialog_photo().is_big;
    case PhotoSizeSource::Type::StickerSetThumbnail:
      return lhs.sticker_set_thumbnail().sticker_set_id == rhs.sticker_set_thumbnail().sticker_set_id &&
             lhs.sticker_set_thumbnail().sticker_set_access_hash == rhs.sticker_set_thumbnail().sticker_set_access_hash;
    case PhotoSizeSource::Type::Empty:
    default:
      return true;
  }
}

bool operator!=(const PhotoSizeSource &lhs, const PhotoSizeSource &rhs) {
  return !(lhs == rhs);
}

}  // namespace td
