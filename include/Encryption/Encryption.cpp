#include "Encryption.h"
#include "Base64/base64.h"
#include "zlib.h"
#include <sstream>


std::string Encryption::Base64::Encode(std::string t)
{
    return base64_encode(t);
}

std::string Encryption::Base64::Encode(std::string t, bool url)
{
    return base64_encode(t, url);
}

std::string Encryption::Base64::Decode(std::string t)
{
    return base64_decode(t);
}

std::string Encryption::Base64::Decode(std::string t, bool url)
{
    return base64_decode(t, url);
}

std::string Encryption::ZLib::Deflate(std::string t)
{
    // Copyright 2007 Timo Bingmann <tb@panthema.net>
    // Distributed under the Boost Software License, Version 1.0.
    // (See http://www.boost.org/LICENSE_1_0.txt)
    //
    // Original link http://panthema.net/2007/0328-ZLibString.html

    int compressionlevel = Z_BEST_COMPRESSION;
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, compressionlevel) != Z_OK)
    {
        std::cout << "[String] Error with deflating string!" << std::endl;
    }

    zs.next_in = (Bytef*)t.data();
    zs.avail_in = t.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;


    do 
    {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) 
        {
            outstring.append(outbuffer,
                zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) 
    {
        std::ostringstream oss;
        oss << "[String] Exception during zlib compression: (" << ret << ") " << zs.msg;
        std::cout << oss.str() << std::endl;
    }

    return outstring;
}

std::string Encryption::ZLib::Inflate(std::string t, ZLib::Format format)
{

    // Copyright 2007 Timo Bingmann <tb@panthema.net>
    // Distributed under the Boost Software License, Version 1.0.
    // (See http://www.boost.org/LICENSE_1_0.txt)
    //
    // Original link http://panthema.net/2007/0328-ZLibString.html

    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    auto wbits = -MAX_WBITS;

    if (format == ZLib::Format::DEFLATE)
        wbits = -MAX_WBITS;
    else if (format == ZLib::Format::ZLIB)
        wbits = MAX_WBITS;
    else if (format == ZLib::Format::GZIP)
        wbits = MAX_WBITS | 16;

    if (inflateInit2(&zs, wbits) != Z_OK)
    {
        std::cout << "[String] Error with inflating string!" << std::endl;
    }

    zs.next_in = (Bytef*)t.data();
    zs.avail_in = t.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    do 
    {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) 
        {
            outstring.append(outbuffer,
                zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) 
    {
        std::ostringstream oss;
        oss << "[String] Exception during zlib decompression: (" << ret << ") "
            << zs.msg;
        std::cout << oss.str() << std::endl;
    }

    return outstring;
}
