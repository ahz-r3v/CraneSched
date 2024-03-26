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

#include "LicenseManager.h"

namespace Ctld {

LicenseManager::LicenseManager() {
    for (auto& lic : g_config.LocalLicenses) {
        m_license_map_[lic.license_name] = std::make_unique<License>(lic);
    }
}

Bool LicenseManager::AddLicense(License&& lic) {
    m_rw_license_mutex_.lock_shared();

    std::string target_name = lic.name;
    auto it = m_license_map_.find(target_name);
    if (it != m_license_map_.end()) {
        m_rw_license_mutex_.unlock_shared();
        return false;
    } else {
        m_license_map_[lic.name] = std::make_unique<License>(lic);
    }
    m_rw_license_mutex_.unlock_shared();
    return true;
}

Bool LicenseManager::AddLicense(std::string license_name, uint64_t total_count, uint64_t used_count) {
    m_rw_license_mutex_.lock_shared();

    auto it = m_license_map_.find(license_name);
    if (it != m_license_map_.end()) {
        m_rw_license_mutex_.unlock_shared();
        return false;
    } else {
        License new_license;
        new_license.name = license_name;
        new_license.total_count = total_count;
        new_license.used_count = used_count;
        new_license.free_count = total_count - used_count;
        m_license_map_[license_name] = std::make_unique<License>(std::move(new_license));
    }
    m_rw_license_mutex_.unlock_shared();
    return true;
}

Bool LicenseManager::SetLicenseTotalCount(std::string license_name, uint64_t total_count) {
    m_rw_license_mutex_.lock_shared();
    
    auto it = m_license_map_.find(license_name);
    if (it != m_license_map_.end()) {
        it->second.total_count = lic.total_count;
        it->second.free_count = lic.total_count - it->second.used_count;
        m_rw_license_mutex_.unlock_shared();
        return true;
    } else {
        m_rw_license_mutex_.unlock_shared();
        return false;
    }
}

Bool LicenseManager::SetLicenseUsedCount(std::string license_name, uint64_t used_count) {
    m_rw_license_mutex_.unlock_shared();

    auto it = m_license_map_.find(license_name);
    if (it != m_license_map_.end()) {
        it->second.used_count = lic.used_count;
        it->second.free_count = it->second.total_count - lic.used_count;
        m_rw_license_mutex_.unlock_shared();
        return true;
    } else {
        m_rw_license_mutex_.unlock_shared();
        return false;
    }
}

Bool LicenseManager::UseLicense(std::string license_name; uint64_t count) {
    m_rw_license_mutex_.unlock_shared();

    auto it = m_license_map_.find(license_name);
    if (it != m_license_map_.end()) {
        if it->second.free_count < count {
            m_rw_license_mutex_.unlock_shared();
            return false;
        } else {
            it->second.free_count -= count;
            it->second.used_count += count;
        }
        m_rw_license_mutex_.unlock_shared();
        return true;
    } else {
        m_rw_license_mutex_.unlock_shared();
        return false;
    }
}

LicenseMutexSharedPtr LicenseManager::GetLicense(std::string license_name) {
    m_rw_license_mutex_.lock_shared();

    auto it = m_license_map_.find(license_name);
    if (it != m_license_map_.end()) {
        m_rw_license_mutex_.unlock_shared();
        return LicenseMutexSharedPtr{it->second, &m_rw_license_mutex_};
    } else {
        m_rw_license_mutex_.unlock_shared();
        return LicenseMutexSharedPtr{nullptr};
    }
}

LicenseMapMutexSharedPtr LicenseManager::GetAllLicenses() {
    m_rw_license_mutex_.lock_shared();

    if (m_license_map_.empty()) {
    m_rw_license_mutex_.unlock_shared();
    return UserMapMutexSharedPtr{nullptr};
  } else {
    m_rw_license_mutex_.unlock_shared();
    return LicenseMapMutexSharedPtr{&m_license_map_, &m_rw_license_mutex_};
  }
}

Bool LicenseManager::DeleteLicense(std::string license_name) {
    m_rw_license_mutex_.lock_shared();

    auto it = m_license_map_.find(license_name);
    if (it != m_license_map_.end()) {
        m_license_map_.erase(it);
        m_rw_license_mutex_.unlock_shared();
        return true;
    } else {
        m_rw_license_mutex_.unlock_shared();
        return false;
    }
}
}