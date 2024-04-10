// Copyright Serinc All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionTarget.h"
#include "Components/ActorComponent.h"
#include "InteractionBaseComponent.generated.h"

class USpringArmComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIS_API UInteractionBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UInteractionBaseComponent();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetInteractionDistance(const float& InValue);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetInteractionDistance() const { return InteractionDistance; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetInteractionCollisionChannel(const TEnumAsByte<ECollisionChannel>& InValue);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSetInteractionDistance(const float& InValue);

	UFUNCTION(BlueprintPure)
	FORCEINLINE TEnumAsByte<ECollisionChannel> GetInteractionCollisionChannel() const { return CollisionChannel; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	TArray<AActor*> GetInteractionFocusedActors() const;

	UFUNCTION(BlueprintCallable)
	virtual void Interact(bool bStart);

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<TScriptInterface<IInteractionTarget>> GetInteractionActors() const { return CurrentInteractionActors; }

	UFUNCTION(BlueprintPure)
	virtual TScriptInterface<IInteractionTarget> GetFirstInteractionActor() const;

	virtual float GetFullInteractionDistance() const;

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;

	UPROPERTY()
	USpringArmComponent* SpringArmComponent;
	
	UFUNCTION(Server, Reliable)
	void ServerInteract(bool bStart);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	float InteractionDistance = 500.0f;

	UPROPERTY(Transient)
	TArray<TScriptInterface<IInteractionTarget>> CurrentInteractionActors;

	USpringArmComponent* FindSpringArmComponent();

};
