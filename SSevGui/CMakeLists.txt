cmake_minimum_required(VERSION 3.5)

project(SSevGui LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DENABLE_ENCODER_GENERIC
	-DENABLE_ENCODER_QR_CODE
	-DENABLE_DECODER_1D_BARCODES
	)

file(GLOB_RECURSE CODE_FILES *.cpp)

SET(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/zh_CN.ts
)

find_package(Qt5LinguistTools)
qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES} OPTIONS -source-language en_US -no-obsolete)
#message(${QM_FILES})


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS DBus REQUIRED)

include_directories("3rd/qzxing")
include_directories("3rd/qzxing/zxing")

set(QZXING_CPP
	3rd/qzxing/CameraImageWrapper.cpp
	3rd/qzxing/QZXing.cpp
	3rd/qzxing/ImageHandler.cpp
	3rd/qzxing/zxing/zxing/ResultIO.cpp
	3rd/qzxing/zxing/zxing/InvertedLuminanceSource.cpp
	3rd/qzxing/zxing/zxing/ChecksumException.cpp
	3rd/qzxing/zxing/zxing/ResultPointCallback.cpp
	3rd/qzxing/zxing/zxing/ResultPoint.cpp
	3rd/qzxing/zxing/zxing/Result.cpp
	3rd/qzxing/zxing/zxing/ResultMetadata.cpp
	3rd/qzxing/zxing/zxing/Reader.cpp
	3rd/qzxing/zxing/zxing/MultiFormatReader.cpp
	3rd/qzxing/zxing/zxing/LuminanceSource.cpp
	3rd/qzxing/zxing/zxing/FormatException.cpp
	3rd/qzxing/zxing/zxing/Exception.cpp
	3rd/qzxing/zxing/zxing/DecodeHints.cpp
	3rd/qzxing/zxing/zxing/BinaryBitmap.cpp
	3rd/qzxing/zxing/zxing/Binarizer.cpp
	3rd/qzxing/zxing/zxing/BarcodeFormat.cpp
	3rd/qzxing/zxing/zxing/ReaderException.cpp
	3rd/qzxing/zxing/zxing/IllegalStateException.cpp
	3rd/qzxing/zxing/zxing/NotFoundException.cpp
	3rd/qzxing/zxing/zxing/WriterException.cpp
	3rd/qzxing/zxing/zxing/common/StringUtils.cpp
	3rd/qzxing/zxing/zxing/common/Str.cpp
	3rd/qzxing/zxing/zxing/common/PerspectiveTransform.cpp
	3rd/qzxing/zxing/zxing/common/IllegalArgumentException.cpp
	3rd/qzxing/zxing/zxing/common/HybridBinarizer.cpp
	3rd/qzxing/zxing/zxing/common/GridSampler.cpp
	3rd/qzxing/zxing/zxing/common/GreyscaleRotatedLuminanceSource.cpp
	3rd/qzxing/zxing/zxing/common/GreyscaleLuminanceSource.cpp
	3rd/qzxing/zxing/zxing/common/GlobalHistogramBinarizer.cpp
	3rd/qzxing/zxing/zxing/common/DetectorResult.cpp
	3rd/qzxing/zxing/zxing/common/DecoderResult.cpp
	3rd/qzxing/zxing/zxing/common/CharacterSetECI.cpp
	3rd/qzxing/zxing/zxing/common/BitSource.cpp
	3rd/qzxing/zxing/zxing/common/BitMatrix.cpp
	3rd/qzxing/zxing/zxing/common/BitArray.cpp
	3rd/qzxing/zxing/zxing/common/BitArrayIO.cpp
	3rd/qzxing/zxing/zxing/common/detector/WhiteRectangleDetector.cpp
	3rd/qzxing/zxing/zxing/common/detector/MonochromeRectangleDetector.cpp
	3rd/qzxing/zxing/zxing/common/reedsolomon/ReedSolomonException.cpp
	3rd/qzxing/zxing/zxing/common/reedsolomon/ReedSolomonDecoder.cpp
	3rd/qzxing/zxing/zxing/common/reedsolomon/GenericGFPoly.cpp
	3rd/qzxing/zxing/zxing/common/reedsolomon/GenericGF.cpp
	3rd/qzxing/zxing/zxing/multi/MultipleBarcodeReader.cpp
	3rd/qzxing/zxing/zxing/multi/GenericMultipleBarcodeReader.cpp
	3rd/qzxing/zxing/zxing/multi/ByQuadrantReader.cpp
	3rd/qzxing/zxing/bigint/BigUnsignedInABase.cc
	3rd/qzxing/zxing/bigint/BigUnsigned.cc
	3rd/qzxing/zxing/bigint/BigIntegerUtils.cc
	3rd/qzxing/zxing/bigint/BigIntegerAlgorithms.cc
	3rd/qzxing/zxing/bigint/BigInteger.cc
##ENABLE_ENCODER_QR_CODE
	3rd/qzxing/zxing/zxing/qrcode/encoder/ByteMatrix.cpp
	3rd/qzxing/zxing/zxing/qrcode/encoder/QREncoder.cpp
	3rd/qzxing/zxing/zxing/qrcode/encoder/MaskUtil.cpp
	3rd/qzxing/zxing/zxing/qrcode/encoder/MatrixUtil.cpp
	3rd/qzxing/zxing/zxing/qrcode/encoder/QRCode.cpp

	3rd/qzxing/zxing/zxing/qrcode/QRVersion.cpp
	3rd/qzxing/zxing/zxing/qrcode/QRFormatInformation.cpp
	3rd/qzxing/zxing/zxing/qrcode/QRErrorCorrectionLevel.cpp
	3rd/qzxing/zxing/zxing/qrcode/decoder/QRMode.cpp
##ENABLE_ENCODER_GENERIC
	3rd/qzxing/zxing/zxing/EncodeHint.cpp
	3rd/qzxing/zxing/zxing/UnsupportedEncodingException.cpp
	3rd/qzxing/zxing/zxing/common/reedsolomon/ReedSolomonEncoder.cpp

##ENABLE_DECODER_1D_BARCODES
	3rd/qzxing/zxing/zxing/oned/UPCEReader.cpp
	3rd/qzxing/zxing/zxing/oned/UPCEANReader.cpp
	3rd/qzxing/zxing/zxing/oned/UPCEANExtensionSupport.cpp
	3rd/qzxing/zxing/zxing/oned/UPCEANExtension2Support.cpp
	3rd/qzxing/zxing/zxing/oned/UPCEANExtension5Support.cpp
	3rd/qzxing/zxing/zxing/oned/UPCAReader.cpp
	3rd/qzxing/zxing/zxing/oned/OneDResultPoint.cpp
	3rd/qzxing/zxing/zxing/oned/OneDReader.cpp
	3rd/qzxing/zxing/zxing/oned/MultiFormatUPCEANReader.cpp
	3rd/qzxing/zxing/zxing/oned/MultiFormatOneDReader.cpp
	3rd/qzxing/zxing/zxing/oned/ITFReader.cpp
	3rd/qzxing/zxing/zxing/oned/EAN13Reader.cpp
	3rd/qzxing/zxing/zxing/oned/EAN8Reader.cpp
	3rd/qzxing/zxing/zxing/oned/EANManufacturerOrgSupport.cpp
	3rd/qzxing/zxing/zxing/oned/Code128Reader.cpp
	3rd/qzxing/zxing/zxing/oned/Code39Reader.cpp
	3rd/qzxing/zxing/zxing/oned/CodaBarReader.cpp
	3rd/qzxing/zxing/zxing/oned/Code93Reader.cpp
	3rd/qzxing/zxing/zxing/oned/rss/AbstractRSSReader.cpp
	3rd/qzxing/zxing/zxing/oned/rss/DataCharacter.cpp
	3rd/qzxing/zxing/zxing/oned/rss/FinderPattern.cpp
	3rd/qzxing/zxing/zxing/oned/rss/Pair.cpp
	3rd/qzxing/zxing/zxing/oned/rss/RSS14Reader.cpp
	3rd/qzxing/zxing/zxing/oned/rss/RSSUtils.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/BitArrayBuilder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/ExpandedPair.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/ExpandedRow.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/RSSExpandedReader.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AbstractExpandedDecoder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AI01AndOtherAIs.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AI01decoder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AI01weightDecoder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AI013x0x1xDecoder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AI013x0xDecoder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AI01320xDecoder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AI01392xDecoder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AI01393xDecoder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AI013103decoder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/AnyAIDecoder.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/BlockParsedResult.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/CurrentParsingState.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/DecodedChar.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/DecodedInformation.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/DecodedNumeric.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/DecodedObject.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/FieldParser.cpp
	3rd/qzxing/zxing/zxing/oned/rss/expanded/decoders/GeneralAppIdDecoder.cpp
	)

add_executable(SSevGui
  main.cpp
  MainWindow.cpp
  connection.cpp
  connectionitem.cpp
  connectiontablemodel.cpp
  statusnotifier.cpp
  ConnectionInfoWgt.cpp
  Settings.cpp
  SubscriptionManagerWgt.cpp
  BusView.cpp
  DataDefine.cpp
  ConnectionTest.cpp
  SSevGUI.qrc
  Tools.cpp
  ShareDialog.cpp
  ${QM_FILES}
  ${QZXING_CPP}
)

target_link_libraries(SSevGui PRIVATE Qt5::Widgets)
target_link_libraries(SSevGui PRIVATE Qt5::Network)
target_link_libraries(SSevGui PRIVATE Qt5::DBus)
