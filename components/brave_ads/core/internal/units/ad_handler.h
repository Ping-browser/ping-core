/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_UNITS_AD_HANDLER_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_UNITS_AD_HANDLER_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "brave/components/brave_ads/core/internal/catalog/catalog.h"
#include "brave/components/brave_ads/core/internal/conversions/conversions.h"
#include "brave/components/brave_ads/core/internal/conversions/conversions_observer.h"
#include "brave/components/brave_ads/core/internal/geographical/region/region.h"
#include "brave/components/brave_ads/core/internal/geographical/subdivision/subdivision.h"
#include "brave/components/brave_ads/core/internal/targeting/behavioral/anti_targeting/resource/anti_targeting_resource.h"
#include "brave/components/brave_ads/core/internal/targeting/behavioral/multi_armed_bandits/epsilon_greedy_bandit_processor.h"
#include "brave/components/brave_ads/core/internal/targeting/behavioral/multi_armed_bandits/resource/epsilon_greedy_bandit_resource.h"
#include "brave/components/brave_ads/core/internal/targeting/behavioral/purchase_intent/purchase_intent_processor.h"
#include "brave/components/brave_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_resource.h"
#include "brave/components/brave_ads/core/internal/targeting/contextual/text_classification/resource/text_classification_resource.h"
#include "brave/components/brave_ads/core/internal/targeting/contextual/text_classification/text_classification_processor.h"
#include "brave/components/brave_ads/core/internal/targeting/contextual/text_embedding/resource/text_embedding_resource.h"
#include "brave/components/brave_ads/core/internal/targeting/contextual/text_embedding/text_embedding_processor.h"
#include "brave/components/brave_ads/core/internal/targeting/geographical/subdivision/subdivision_targeting.h"
#include "brave/components/brave_ads/core/internal/transfer/transfer.h"
#include "brave/components/brave_ads/core/internal/transfer/transfer_observer.h"
#include "brave/components/brave_ads/core/internal/units/inline_content_ad/inline_content_ad_handler.h"
#include "brave/components/brave_ads/core/internal/units/new_tab_page_ad/new_tab_page_ad_handler.h"
#include "brave/components/brave_ads/core/internal/units/notification_ad/notification_ad_handler.h"
#include "brave/components/brave_ads/core/internal/units/promoted_content_ad/promoted_content_ad_handler.h"
#include "brave/components/brave_ads/core/internal/units/search_result_ad/search_result_ad_handler.h"
#include "brave/components/brave_ads/core/public/ads_callback.h"

namespace brave_ads {

class Account;
class Transfer;
struct AdInfo;
struct ConversionInfo;

class AdHandler final : public ConversionsObserver, TransferObserver {
 public:
  explicit AdHandler(Account& account);

  AdHandler(const AdHandler&) = delete;
  AdHandler& operator=(const AdHandler&) = delete;

  AdHandler(AdHandler&&) noexcept = delete;
  AdHandler& operator=(AdHandler&&) noexcept = delete;

  ~AdHandler() override;

  void MaybeServeInlineContentAd(const std::string& dimensions,
                                 MaybeServeInlineContentAdCallback callback);
  void TriggerInlineContentAdEvent(const std::string& placement_id,
                                   const std::string& creative_instance_id,
                                   mojom::InlineContentAdEventType event_type,
                                   TriggerAdEventCallback callback);

  void MaybeServeNewTabPageAd(MaybeServeNewTabPageAdCallback callback);
  void TriggerNewTabPageAdEvent(const std::string& placement_id,
                                const std::string& creative_instance_id,
                                mojom::NewTabPageAdEventType event_type,
                                TriggerAdEventCallback callback);

  void TriggerNotificationAdEvent(const std::string& placement_id,
                                  mojom::NotificationAdEventType event_type,
                                  TriggerAdEventCallback callback);

  void TriggerPromotedContentAdEvent(
      const std::string& placement_id,
      const std::string& creative_instance_id,
      mojom::PromotedContentAdEventType event_type,
      TriggerAdEventCallback callback);

  void TriggerSearchResultAdEvent(mojom::SearchResultAdInfoPtr ad_mojom,
                                  mojom::SearchResultAdEventType event_type,
                                  TriggerAdEventCallback callback);

 private:
  // ConversionsObserver:
  void OnDidConvertAd(const ConversionInfo& conversion) override;

  // TransferObserver:
  void OnDidTransferAd(const AdInfo& ad) override;

  const raw_ref<Account> account_;

  Catalog catalog_;

  Conversions conversions_;

  Transfer transfer_;

  Region region_;
  SubdivisionTargeting subdivision_targeting_;
  Subdivision subdivision_;

  AntiTargetingResource anti_targeting_resource_;

  PurchaseIntentResource purchase_intent_resource_;
  PurchaseIntentProcessor purchase_intent_processor_;

  EpsilonGreedyBanditResource epsilon_greedy_bandit_resource_;
  EpsilonGreedyBanditProcessor epsilon_greedy_bandit_processor_;

  TextClassificationResource text_classification_resource_;
  TextClassificationProcessor text_classification_processor_;

  TextEmbeddingResource text_embedding_resource_;
  TextEmbeddingProcessor text_embedding_processor_;

  InlineContentAdHandler inline_content_ad_handler_;
  NewTabPageAdHandler new_tab_page_ad_handler_;
  NotificationAdHandler notification_ad_handler_;
  PromotedContentAdHandler promoted_content_ad_handler_;
  SearchResultAd search_result_ad_handler_;
};

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_UNITS_AD_HANDLER_H_
