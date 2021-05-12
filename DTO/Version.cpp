/*------------------------------------------------------------------------------
Copyright 2021 Garmin Ltd. or its subsidiaries.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
------------------------------------------------------------------------------*/

/**
    @file
    @brief Represents the ActiveCaptain community database
    version

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "Version"

#include "DBG_pub.h"

#include "Acdb/PrvTypes.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/Version.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
Version::Version() : mVersionParts(CountVersionParts, 0) {}  // end of Version::Version

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
Version::Version(const std::string& aIn) : Version() { Set(aIn); }  // end of Version::Version

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!     Determine if the provided database version is invalid.
//!
//!   @return
//!     if the passed in version is invalid.
//!
//----------------------------------------------------------------
bool Version::Invalid() const {
  Version invalidVersion;
  return (mVersionParts == invalidVersion.mVersionParts);
}  // end of Version::Invalid

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!     Determine if the database version has a schema compatible
//!     but newer version than the provided one.
//!
//----------------------------------------------------------------
bool Version::IsNewerThan(const Version& aOtherVersion) const {
  return !Invalid() &&
         (mVersionParts[SchemaDatabaseVersionIndex] ==
          aOtherVersion.mVersionParts[SchemaDatabaseVersionIndex]) &&
         (mVersionParts[FullDownloadDatabaseVersionIndex] >
          aOtherVersion.mVersionParts[FullDownloadDatabaseVersionIndex]);
}  // end of Version::IsNewerThan

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!     Determine if the provided database version has
//!     a compatible schema with the one represented by
//!     this object.
//!
//!   @return
//!     Are the two version compatible for the purpose of
//!     reading the schema
//!
//----------------------------------------------------------------
bool Version::SchemaCompatible() const {
  Version supportSchema(SupportedSchemaVer);

  return (!Invalid() && (mVersionParts[SchemaDatabaseVersionIndex] ==
                         supportSchema.mVersionParts[SchemaDatabaseVersionIndex]));
}  // end of Version::SchemaCompatible

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Creates a version string.
//!
//!   @returns the version string.
//!
//----------------------------------------------------------------
std::string Version::ToString() const {
  return String::Format("%u.%u.%u.%u", mVersionParts[SchemaDatabaseVersionIndex],
                        mVersionParts[FullDownloadDatabaseVersionIndex],
                        mVersionParts[BuildDateDatabaseVersionIndex],
                        mVersionParts[BuildDateCountDatabaseVersionIndex]);
}  // end of Version::ToString

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Unrolls the passed in string to the component
//!           version parts
//!
//----------------------------------------------------------------
void Version::Set(const std::string& aIn) {
  std::vector<std::string> parts = String::Split(aIn, '.', CountVersionParts);
  if (parts.size() == CountVersionParts) {
    mVersionParts[SchemaDatabaseVersionIndex] = String::ToInt(parts[SchemaDatabaseVersionIndex]);
    mVersionParts[FullDownloadDatabaseVersionIndex] =
        String::ToInt(parts[FullDownloadDatabaseVersionIndex]);
    mVersionParts[BuildDateDatabaseVersionIndex] =
        String::ToInt(parts[BuildDateDatabaseVersionIndex]);
    mVersionParts[BuildDateCountDatabaseVersionIndex] =
        String::ToInt(parts[BuildDateCountDatabaseVersionIndex]);
  } else {
    DBG_W(
        "Active Captain community database version string invalid expecting format <SCHEMA VERSION>.<FULL DOWNLOAD VERSION>.<BUILD DATE>.<BUILD COUNT ON BUILD DATE> version string = %s",
        aIn.c_str());
  }

}  // end of Version::Set

//----------------------------------------------------------------
//!
//!   @public
//!   @detail operator ==
//!
//----------------------------------------------------------------
bool Version::operator==(const Version& aItem) const {
  return (aItem.mVersionParts == mVersionParts);
}  // end of operator ==

//----------------------------------------------------------------
//!
//!   @public
//!   @detail operator >
//!
//----------------------------------------------------------------
bool Version::operator>(const Version& aItem) const {
  return ((mVersionParts[FullDownloadDatabaseVersionIndex] >
           aItem.mVersionParts[FullDownloadDatabaseVersionIndex]) ||
          (mVersionParts[BuildDateDatabaseVersionIndex] >
           aItem.mVersionParts[BuildDateDatabaseVersionIndex]) ||
          ((mVersionParts[BuildDateDatabaseVersionIndex] ==
            aItem.mVersionParts[BuildDateDatabaseVersionIndex]) &&
           (mVersionParts[BuildDateCountDatabaseVersionIndex] >
            aItem.mVersionParts[BuildDateCountDatabaseVersionIndex])));
}  // end of operator >
}  // end of namespace Acdb
