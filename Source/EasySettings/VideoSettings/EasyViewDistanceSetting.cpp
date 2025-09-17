#include "EasyViewDistanceSetting.h"
#include "GameFramework/GameUserSettings.h"

void UEasyViewDistanceSetting::SetValue_Logic_Implementation(int NewValue)
{
    UGameUserSettings::GetGameUserSettings()->SetViewDistanceQuality(NewValue);
    OnSettingChanged.Broadcast(this);
    OnValueChanged.Broadcast(NewValue);
}

int UEasyViewDistanceSetting::GetValue_Implementation() const
{
    return UGameUserSettings::GetGameUserSettings()->GetViewDistanceQuality();
}

UEasyViewDistanceSetting::UEasyViewDistanceSetting()
{
    SettingName = NSLOCTEXT("EasySettings", "ViewDistanceSetting", "View Distance Quality");
}

bool UEasyViewDistanceSetting::IsDisplayConditionMet_Implementation()
{
    return Super::IsDisplayConditionMet_Implementation();
}
