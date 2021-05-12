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
    Class that represents the database version.

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_Version_hpp
#define ACDB_Version_hpp

#include <inttypes.h>
#include <string>
#include <vector>

namespace Acdb {
class Version {
 public:
  // functions
  Version();

  Version(const std::string& aIn);

  bool Invalid() const;

  bool IsNewerThan(const Version& aOtherVersion) const;

  bool SchemaCompatible() const;

  std::string ToString() const;

  void Set(const std::string& aIn);

  bool operator==(const Version& aItem) const;

  bool operator>(const Version& aItem) const;

 private:
  // enums
  //! SchemaDatabaseVersionIndex this version increments if the schema
  //! changes in a non-backwards compatible manner.  It does not increment
  //! for every schema change.
  //! FullDownloadDatabaseVersionIndex this version increments if the database
  //! has changed in a manner such that it is better to download a new copy
  //! instead of doing a delta load.  This would be the case if every row of the
  //! database was updated because a column was added.
  //! BuildDateDatabaseVersionIndex represents the build date in text YYYYMMDD
  //! and is then mapped to a uint32
  //! BuildDateCountDatabaseVersionIndex it is possible the database will have to
  //! be constructed more than once in a day.  This is the count on the day the
  //! database was built.
  enum DatabaseVersionsSerialization : uint32_t {
    SchemaDatabaseVersionIndex = 0,
    FullDownloadDatabaseVersionIndex = 1,
    BuildDateDatabaseVersionIndex = 2,
    BuildDateCountDatabaseVersionIndex = 3,

    CountVersionParts = 4
  };

  // Variables
  std::vector<uint32_t> mVersionParts;
};  // end of class Version

}  // end of namespace Acdb

#endif  // end of ACDB_Version_hpp
