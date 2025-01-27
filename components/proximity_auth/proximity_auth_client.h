// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_PROXIMITY_AUTH_PROXIMITY_AUTH_CLIENT_H_
#define COMPONENTS_PROXIMITY_AUTH_PROXIMITY_AUTH_CLIENT_H_

#include <memory>
#include <string>

#include "base/callback_forward.h"
#include "components/cryptauth/proto/cryptauth_api.pb.h"
#include "components/proximity_auth/screenlock_state.h"

class PrefService;

namespace cryptauth {
class CryptAuthClientFactory;
class CryptAuthDeviceManager;
class CryptAuthEnrollmentManager;
class SecureMessageDelegate;
}

namespace proximity_auth {

// An interface that needs to be supplied to the Proximity Auth component by its
// embedder. There should be one |ProximityAuthClient| per
// |content::BrowserContext|.
class ProximityAuthClient {
 public:
  virtual ~ProximityAuthClient() {}

  // Returns the authenticated username.
  virtual std::string GetAuthenticatedUsername() const = 0;

  // Updates the user pod on the signin or lock screen to reflect the provided
  // screenlock state.
  virtual void UpdateScreenlockState(ScreenlockState state) = 0;

  // Finalizes an unlock attempt initiated by the user. If |success| is true,
  // the screen is unlocked; otherwise, the auth attempt is rejected. An auth
  // attempt must be in progress before calling this function.
  virtual void FinalizeUnlock(bool success) = 0;

  // Finalizes a sign-in attempt initiated by the user. If |secret| is valid,
  // the user is signed in; otherwise, the auth attempt is rejected. An auth
  // attempt must be in progress before calling this function.
  virtual void FinalizeSignin(const std::string& secret) = 0;

  // Gets the wrapped challenge for the given |user_id| and |remote_public_key|
  // of the user's remote device. The challenge binds to the secure channel
  // using |channel_binding_data|.
  // |callback| will be invoked when the challenge is acquired.
  virtual void GetChallengeForUserAndDevice(
      const std::string& user_id,
      const std::string& remote_public_key,
      const std::string& channel_binding_data,
      base::Callback<void(const std::string& challenge)> callback) = 0;

  // Returns the PrefService used by the profile.
  virtual PrefService* GetPrefService() = 0;

  // Returns the SecureMessageDelegate used by the system.
  virtual std::unique_ptr<cryptauth::SecureMessageDelegate>
  CreateSecureMessageDelegate() = 0;

  // Constructs the CryptAuthClientFactory that can be used for API requests.
  virtual std::unique_ptr<cryptauth::CryptAuthClientFactory>
  CreateCryptAuthClientFactory() = 0;

  // Constructs the DeviceClassifier message that is sent to CryptAuth for all
  // API requests.
  virtual cryptauth::DeviceClassifier GetDeviceClassifier() = 0;

  // Returns the account id of the user.
  virtual std::string GetAccountId() = 0;

  virtual cryptauth::CryptAuthEnrollmentManager*
  GetCryptAuthEnrollmentManager() = 0;

  virtual cryptauth::CryptAuthDeviceManager* GetCryptAuthDeviceManager() = 0;

  virtual std::string GetLocalDevicePublicKey() = 0;
};

}  // namespace proximity_auth

#endif  // COMPONENTS_PROXIMITY_AUTH_PROXIMITY_AUTH_CLIENT_H_
