#include "utils.h"

#include <cstdlib>

#include "mbdtls.h"
std::string RandomString(uint8_t seed, uint8_t length) {
    srand(seed);
    const char *letters[40] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l",
                               "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
                               "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    std::string randString = "";
    for (int i = 0; i < length; i++) {
        randString.append(letters[rand() % 10]);
    }
    return randString;
}

String TimeToStr(uint8_t hours, uint8_t minutes) {
    String result = "";
    if (hours < 10) result += "0";
    result += hours;
    result += ":";
    if (minutes < 10) result += "0";
    result += minutes;
    return result;
}

String Sha256(String data) {
    const char *payload = data.c_str();
    byte shaResult[32];

    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    const size_t payloadLength = strlen(payload);

    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, (const unsigned char *)payload, payloadLength);
    mbedtls_md_finish(&ctx, shaResult);
    mbedtls_md_free(&ctx);

    Serial.print("Hash: ");
    String result = "";
    for (int i = 0; i < sizeof(shaResult); i++) {
        char str[3];
        snprintf(str, sizeof(shaResult), "%02x", static_cast<int>(shaResult[i]));
        Serial.print(str);
        result = result + str;
    }
    return result;
}
