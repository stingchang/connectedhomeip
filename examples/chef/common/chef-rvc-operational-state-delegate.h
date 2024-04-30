/*
 *
 *    Copyright (c) 2023 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once

#include <app-common/zap-generated/cluster-objects.h>
#include <app/clusters/operational-state-server/operational-state-server.h>
#include <protocols/interaction_model/StatusCode.h>

using chip::Protocols::InteractionModel::Status;

namespace chip {
namespace app {
namespace Clusters {

namespace RvcOperationalState {

// This is an application level delegate to handle operational state commands according to the specific business logic.
class RvcOperationalStateDelegate : public chip::app::Clusters::RvcOperationalState::Delegate
{
private:
    const OperationalState::OperationalStateEnum rvcOpStateList[7] = {
        OperationalState::OperationalStateEnum(to_underlying(OperationalState::OperationalStateEnum::kStopped)),
        OperationalState::OperationalStateEnum(to_underlying(OperationalState::OperationalStateEnum::kRunning)),
        OperationalState::OperationalStateEnum(to_underlying(OperationalState::OperationalStateEnum::kPaused)),
        OperationalState::OperationalStateEnum(to_underlying(OperationalState::OperationalStateEnum::kError)),
        OperationalState::OperationalStateEnum(
            to_underlying(Clusters::RvcOperationalState::OperationalStateEnum::kSeekingCharger)),
        OperationalState::OperationalStateEnum(to_underlying(Clusters::RvcOperationalState::OperationalStateEnum::kCharging)),
        OperationalState::OperationalStateEnum(to_underlying(Clusters::RvcOperationalState::OperationalStateEnum::kDocked)),
    };

public:
    RvcOperationalStateDelegate()
    {
        mOperationalStateList = Span<const OperationalState::OperationalStateEnum>(rvcOpStateList);
    }

protected:
    Span<const OperationalState::OperationalStateEnum> mOperationalStateList;
    Span<const CharSpan> mOperationalPhaseList;

};

void Shutdown();

} // namespace RvcOperationalState
} // namespace Clusters
} // namespace app
} // namespace chip


#ifdef MATTER_DM_PLUGIN_RVC_OPERATIONAL_STATE_SERVER
chip::Protocols::InteractionModel::Status 
chefRvcOperationalStateWriteCallback(chip::EndpointId endpoint, chip::ClusterId clusterId,
                                          const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer);
chip::Protocols::InteractionModel::Status
chefRvcOperationalStateReadCallback(chip::EndpointId endpoint, chip::ClusterId clusterId,
                                         const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer,
                                         uint16_t maxReadLength);
#endif // MATTER_DM_PLUGIN_RVC_OPERATIONAL_STATE_SERVER
