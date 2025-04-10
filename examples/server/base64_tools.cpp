#include "base64_tools.h"

bool isValidImageMimeType(const std::string & mime_type) {
    static const std::vector<std::string> valid_types = { "png", "jpeg",    "jpg",  "gif",   "webp",
                                                          "bmp", "svg+xml", "tiff", "x-icon" };

    for (const auto & type : valid_types) {
        if (mime_type == type) {
            return true;
        }
    }

    return false;
}

bool isValidBase64Data(const std::string & data) {
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/=";

    for (char c : data) {
        if (base64_chars.find(c) == std::string::npos) {
            return false;
        }
    }

    if (data.length() % 4 != 0) {
        return false;
    }

    return true;
}

bool isValidDataImageUrl(const std::string & url) {
    const std::string prefix = "data:image/";
    if (url.find(prefix) != 0) {
        return false;
    }

    size_t semicolon_pos = url.find(';', prefix.length());
    if (semicolon_pos == std::string::npos) {
        return false;
    }

    std::string mime_type = url.substr(prefix.length(), semicolon_pos - prefix.length());
    if (mime_type.empty() || !isValidImageMimeType(mime_type)) {
        return false;
    }

    size_t comma_pos = url.find(',', semicolon_pos);
    if (comma_pos == std::string::npos) {
        return false;
    }

    std::string encoding = url.substr(semicolon_pos + 1, comma_pos - semicolon_pos - 1);
    if (encoding != "base64" && !encoding.empty()) {
        return false;
    }

    if (url.length() <= comma_pos + 1) {
        return false;
    }

    std::string data = url.substr(comma_pos + 1);
    if (encoding == "base64" && !isValidBase64Data(data)) {
        return false;
    }

    return true;
}
