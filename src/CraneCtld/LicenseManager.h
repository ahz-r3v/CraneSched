/**
 * Copyright (c) 2023 Peking University and Peking University
 * Changsha Institute for Computing and Digital Economy
 *
 * CraneSched is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of
 * the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS,
 * WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */

#pragma once

#include "CtldPublicDefs.h"
// Precompiled header comes first!

#include "crane/Lock.h"
#include "crane/Pointer.h"

namespace Ctld {

class LicenseManager {
public:
    using LicenseMutexSharedPtr = util::ScopeConstSharedPtr<License, util::rw_mutex>;
    using LicenseMapMutexSharedPtr = util::ScopeConstSharedPtr< std::unordered_map<std::string, std::unique_ptr<License>>, util::rw_mutex>;

    LicenseManager();

    ~LicenseManager() = default;

    Bool AddLicense(License&& lic);

    Bool AddLicense(std::string license_name, uint64_t total_count, uint64_t used_count);

    Bool SetLicenseTotalCount(std::string license_name, uint64_t total_count);

    Bool SetLicenseUsedCount(std::string license_name, uint64_t used_count);

    Bool UseLicense(std::string license_name; uint64_t count);

    LicensePtr GetLicense(std::string license_name);

    LicenseMapMutexSharedPtr GetAllLicenses();

    Bool DeleteLicense(std::string license_name)

private:
    /* <license name, License> */
    std::unordered_map<std::string, std::unique_ptr<License>> m_license_map_;
    /* rwlock for licenses */
    util::rw_mutex m_rw_license_mutex_;
    
}


}  // namespace Ctld

inline std::unique_ptr<Ctld::LicenseManager> g_license_manager;