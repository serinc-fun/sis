// Copyright Serinc All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionBaseComponent.h"
#include "InteractionSingleComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractionSingleComponent : public UInteractionBaseComponent
{
	GENERATED_BODY()

public:
	
	UInteractionSingleComponent();

	virtual TArray<AActor*> GetInteractionFocusedActors_Implementation() const override;
};
