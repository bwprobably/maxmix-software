//********************************************************
// PROJECT: MAXMIX
// AUTHOR: Ruben Henares
// EMAIL: rhenares0@gmail.com
//
// DECRIPTION:
//********************************************************

//********************************************************
// *** FUNCTIONS
//********************************************************
//-----------------------------------------------------------------------------
// \brief Encode a byte buffer with the COBS encoder.
// \param inBuffer A pointer to the unencoded buffer to encode.
// \param size  The number of bytes in the \p inBuffer.
// \param outBuffer The buffer for the encoded bytes.
// \returns The number of bytes written to the \p outBuffer.
// \warning The encodedBuffer must have at least getEncodedBufferSize() 
//          allocated.
//-----------------------------------------------------------------------------
uint8_t Encode(const uint8_t* inBuffer, uint8_t size, uint8_t* outBuffer)
{
    uint8_t inIndex  = 0;
    uint8_t outIndex = 1;
    uint8_t codeIndex  = 0;
    uint8_t code       = 1;

    while (inIndex < size)
    {
        if (inBuffer[inIndex] == 0)
        {
            outBuffer[codeIndex] = code;
            code = 1;
            codeIndex = outIndex++;
            inIndex++;
        }
        else
        {
            outBuffer[outIndex++] = inBuffer[inIndex++];
            code++;

            if (code == 0xFF)
            {
                outBuffer[codeIndex] = code;
                code = 1;
                codeIndex = outIndex++;
            }
        }
    }

    outBuffer[codeIndex] = code;
    return outIndex;
}

//-----------------------------------------------------------------------------
// \brief Decode a COBS-encoded buffer.
// \param inBuffer A pointer to the \p inBuffer to decode.
// \param size The number of bytes in the \p inBuffer.
// \param outBuffer The target buffer for the decoded bytes.
// \returns The number of bytes written to the \p outBuffer.
// \warning decodedBuffer must have a minimum capacity of size.
//-----------------------------------------------------------------------------
uint8_t Decode(const uint8_t* inBuffer, uint8_t size, uint8_t* outBuffer)
{
    if (size == 0)
        return 0;

    uint8_t inIndex = 0;
    uint8_t outIndex = 0;
    uint8_t code = 0;
    uint8_t i = 0;

    while (inIndex < size)
    {
        code = inBuffer[inIndex];
        
        if (inIndex + code > size && code != 1)
            return 0;

        inIndex++;

        for (i = 1; i < code; i++)
            outBuffer[outIndex++] = inBuffer[inIndex++];

        if (code != 0xFF && inIndex != size)
            outBuffer[outIndex++] = '\0';
    }

    return outIndex;
}
