#include "EasyFoliageSetting.h"
#include "GameFramework/GameUserSettings.h"

void UEasyFoliageSetting::SetValue_Logic_Implementation(int NewValue)
{
    UGameUserSettings::GetGameUserSettings()->SetFoliageQuality(NewValue);
    OnSettingChanged.Broadcast(this);
    OnValueChanged.Broadcast(NewValue);
}

int UEasyFoliageSetting::GetValue_Implementation() const
{
    return UGameUserSettings::GetGameUserSettings()->GetFoliageQuality();
}

UEasyFoliageSetting::UEasyFoliageSetting()
{
    SettingName = NSLOCTEXT("EasySettings", "FoliageSetting", "Foliage Quality");
}

bool UEasyFoliageSetting::IsDisplayConditionMet_Implementation()
{
    return Super::IsDisplayConditionMet_Implementation();
}
