/*
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
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/util/config.h>

#ifdef MATTER_DM_PLUGIN_RVC_OPERATIONAL_STATE_SERVER
#include <chef-rvc-operational-state-delegate.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::OperationalState;
using namespace chip::app::Clusters::RvcOperationalState;
using chip::Protocols::InteractionModel::Status;

static RvcOperationalState::Instance * gRvcOperationalStateInstance = nullptr;
static RvcOperationalStateDelegate * gRvcOperationalStateDelegate   = nullptr;

void RvcOperationalState::Shutdown()
{
    if (gRvcOperationalStateInstance != nullptr)
    {
        delete gRvcOperationalStateInstance;
        gRvcOperationalStateInstance = nullptr;
    }
    if (gRvcOperationalStateDelegate != nullptr)
    {
        delete gRvcOperationalStateDelegate;
        gRvcOperationalStateDelegate = nullptr;
    }
}

chip::Protocols::InteractionModel::Status chefRvcOperationalStateWriteCallback(chip::EndpointId endpointId, chip::ClusterId clusterId,
                                          const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer)
{
    chip::Protocols::InteractionModel::Status ret = chip::Protocols::InteractionModel::Status::Success;

    VerifyOrDie(endpointId == 1); // this cluster is only enabled for endpoint 1.
    VerifyOrDie(gRvcOperationalStateInstance != nullptr);

    chip::AttributeId attributeId    = attributeMetadata->attributeId;

    switch (attributeId)
    {
    case chip::app::Clusters::RvcOperationalState::Attributes::CurrentPhase::Id: {
        uint8_t m                           = static_cast<uint8_t>(buffer[0]);
	DataModel::Nullable<uint8_t> aPhase(m);
        CHIP_ERROR err = gRvcOperationalStateInstance->SetCurrentPhase(aPhase);
        if (CHIP_NO_ERROR == err)
        {
            break;
        }
        ret = chip::Protocols::InteractionModel::Status::UnsupportedWrite;
        ChipLogError(DeviceLayer, "Invalid Attribute Update status: %" CHIP_ERROR_FORMAT, err.Format());
    }
    break;
    case chip::app::Clusters::RvcOperationalState::Attributes::OperationalState::Id: {
        uint8_t m                           = static_cast<uint8_t>(buffer[0]);
        CHIP_ERROR err = gRvcOperationalStateInstance->SetOperationalState(m);
        if (CHIP_NO_ERROR == err)
        {
            break;
        }
        ret = chip::Protocols::InteractionModel::Status::UnsupportedWrite;
        ChipLogError(DeviceLayer, "Invalid Attribute Update status: %" CHIP_ERROR_FORMAT, err.Format());
    }
    break;
    default:
        ret = chip::Protocols::InteractionModel::Status::UnsupportedAttribute;
        ChipLogError(DeviceLayer, "Unsupported Attribute ID: %d", static_cast<int>(attributeId));
        break;
    }

    return ret;
}

chip::Protocols::InteractionModel::Status chefRvcOperationalStateReadCallback(chip::EndpointId endpoint, chip::ClusterId clusterId,
                                         const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer,
                                         uint16_t maxReadLength)
{

    return chip::Protocols::InteractionModel::Status::Success;
}

void emberAfRvcOperationalStateClusterInitCallback(chip::EndpointId endpointId)
{
    VerifyOrDie(endpointId == 1); // this cluster is only enabled for endpoint 1.
    VerifyOrDie(gRvcOperationalStateInstance == nullptr && gRvcOperationalStateDelegate == nullptr);

    gRvcOperationalStateDelegate        = new RvcOperationalStateDelegate;
    EndpointId operationalStateEndpoint = 0x01;
    gRvcOperationalStateInstance        = new RvcOperationalState::Instance(gRvcOperationalStateDelegate, operationalStateEndpoint);

    gRvcOperationalStateInstance->SetOperationalState(to_underlying(OperationalState::OperationalStateEnum::kStopped));

    gRvcOperationalStateInstance->Init();
}
#endif // MATTER_DM_PLUGIN_RVC_OPERATIONAL_STATE_SERVER
