// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"

void UAttributeInfo::PostLoad()
{
	Super::PostLoad();

	PopulateDataAsset();
}

void UAttributeInfo::PopulateDataAsset()
{
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();

	for (int i = 0; i < AuraGameplayTags.AuraAllTagsContainer.Num(); ++i)
	{
		const FGameplayTag IndexTag = AuraGameplayTags.AuraAllTagsContainer.GetByIndex(i); // Get indexed GameplayTag
		AttributeInfo[i].AttributeTag = IndexTag; // Set AttributeTag
		
		const FGameplayTagNode* TagNode = UGameplayTagsManager::Get().FindTagNode(IndexTag).Get(); // Get the last node of the GameplayTag  
		FString NodeString = FName::NameToDisplayString(TagNode->GetSimpleTagName().ToString(), false); // Get node name and make it display ready
		AttributeInfo[i].AttributeName = FText::FromString(NodeString); // Set AttributeName
		
		const FName TagName = IndexTag.GetTagName(); // Get GameplayTag name
		/** OUTs */
		FString TagComment;
		FName TagSource;
		bool bIsTagExplicit;
		bool bIsRestrictedTag;
		bool bAllowNonRestrictedChildren;

		// Get Editor Data from TagName
		UGameplayTagsManager::Get().GetTagEditorData
		(
			TagName,
			TagComment,
			TagSource,
			bIsTagExplicit,
			bIsRestrictedTag,
			bAllowNonRestrictedChildren
		);
		AttributeInfo[i].AttributeDescription = FText::FromString(TagComment); // Set AttributeDescription

		// Another way of getting the GameplayTag name for the last node
		/*FString MyStr = IndexTag.ToString();
		constexpr TCHAR PeriodChar = '.';
		int32 PeriodIndex;
		MyStr.FindLastChar(PeriodChar, PeriodIndex);
		MyStr.RightChopInline(PeriodIndex);
		MyStr.RemoveAt(0);

		// Add space between words
		for (int j = 0; j < MyStr.Len(); ++j)
		{
			if (std::iswupper(MyStr[j]) && j > 0)
			{
				constexpr TCHAR SpaceChar = ' ';
				MyStr.InsertAt(j, SpaceChar);
				++j;
			}
		}*/
	}
}

FAuraAttributeInfo UAttributeInfo::GetAttributeInfoByTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& Info : AttributeInfo)
	{
		if (Info.AttributeTag.MatchesTagExact(Tag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag: [%s], on AttributeInfo: [%s]"), *Tag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}