// Copyright Serinc All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionTarget.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractionTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONSYSTEM_API IInteractionTarget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void OnInteractionFocusStart(APawn* InInstigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnInteractionFocusLost(APawn* InInstigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnInteractionStart(APawn* InInstigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnInteractionStop(APawn* InInstigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsInteractionAllowed(APawn* InInstigator) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FText GetInteractionTitle(APawn* InInstigator) const;
};
