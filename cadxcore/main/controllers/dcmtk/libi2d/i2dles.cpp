/*
 *
 *  $Id$
 *  Ginkgo CADx Project
 *
 *  Code addapted from DCMTK
 *
 */
#include "i2dles.h"


OFString I2DLittleEndianSource::inputFormat() const
{
        return "LittleEndian";
}

/** Extracts the raw JPEG pixel data stream from a JPEG file and returns some
 *  image information about this pixel data.
 *  Raw means here that all APP markers (e.g. JFIF information) are removed from the JPEG stream.
 *  The pixel data returned is a JPEG stream in JPEG interchange format.
 *  This function allocates memory for the pixel data returned to the user. The caller of this
 *  function is responsible for deleting the memory buffer
 *  @param rows - [out] Rows of image
 *  @param cols - [out] Columns of image
 *  @param samplesPerPixel - [out] Number of components per pixel
 *  @param photoMetrInt - [out] The DICOM color model used for the compressed data
 *  @param bitsAlloc - [out] Bits Allocated for one sample
 *  @param bitsStored - [out] Bits Stored, number of bits stored within Bits Allocated
 *  @param highBit - [out] High Bit, hightest bit position set within Bits Allocated
 *  @param pixelRepr - [out] Pixel Representation (0=unsigned, 1=signed)
 *  @param planConf - [out] Planar Configuration
 *  @param pixAspectH - [out] Horizontal value of pixel aspect ratio
 *  @param pixAspectV - [out] Vertical value of pixel aspect ratio
 *  @param pixData - [out] Pointer to the pixel data in JPEG Interchange Format (but without APPx markers).
 *  @param length - [out] Length of pixel data
 *  @param ts - [out] The transfer syntax imposed by the imported pixel pixel data.
                      This is necessary for the JPEG importer that needs to report
                      which TS must be used for the imported JPEG data (ie. baseline, progressive, ...).
                      If pixel data is uncompressed, EXS_Unknown is returned
 *  @return EC_Normal, if successful, error otherwise
 */
OFCondition I2DLittleEndianSource::readPixelData( Uint16& rows,
                Uint16& cols,
                Uint16& samplesPerPixel,
                OFString& photoMetrInt,
                Uint16& bitsAlloc,
                Uint16& bitsStored,
                Uint16& highBit,
                Uint16& pixelRepr,
                Uint16& planConf,
                Uint16& pixAspectH,
                Uint16& pixAspectV,
                char*&  pixData,
                Uint32& length,
                E_TransferSyntax& ts)
{
        photoMetrInt = m_ImgFile.photometricInterpretation.c_str();

        cols = m_ImgFile.width;
        rows = m_ImgFile.heigth;

        samplesPerPixel = m_ImgFile.samplesPerPixel;
        bitsAlloc = m_ImgFile.bitsAllocated;
        bitsStored = m_ImgFile.bitsStored;
        highBit = m_ImgFile.highBit;
        pixelRepr = m_ImgFile.pixelRepr;
        planConf = m_ImgFile.planarConf;
        pixAspectH = m_ImgFile.pixAspectH;
        pixAspectV = m_ImgFile.pixAspectV;
        pixData = (char*)m_ImgFile.pixelData;
        if (bitsAlloc <= 8) {
                length = cols * rows * samplesPerPixel;
        } else if (bitsAlloc <= 16) {
                length = cols * rows * samplesPerPixel * 2;
        } else if (bitsAlloc <= 24) {
                length = cols * rows * samplesPerPixel * 3;
        } else if (bitsAlloc <= 32) {
                length = cols * rows * samplesPerPixel * 4;
        } else if (bitsAlloc <= 48) {
                length = cols * rows * samplesPerPixel * 6;
        } else if (bitsAlloc <= 64) {
                length = cols * rows * samplesPerPixel * 8;
        }

        ts = EXS_LittleEndianExplicit;

        return EC_Normal;
}
