// Copyright Serinc All Rights Reserved.

#include "InteractionBaseComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

UInteractionBaseComponent::UInteractionBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UInteractionBaseComponent::SetInteractionDistance(const float& InValue)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		InteractionDistance = InValue;
	}
}

void UInteractionBaseComponent::SetInteractionCollisionChannel(const TEnumAsByte<ECollisionChannel>& InValue)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		CollisionChannel = InValue;
	}
}

void UInteractionBaseComponent::ServerSetInteractionDistance_Implementation(const float& InValue)
{
	SetInteractionDistance(InValue);
}

TArray<AActor*> UInteractionBaseComponent::GetInteractionFocusedActors_Implementation() const
{
	return {};
}

void UInteractionBaseComponent::Interact(bool bStart)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		ServerInteract(bStart);
	}

	const auto LOwner = GetOwner<APawn>();	
	for (auto LInteraction : CurrentInteractionActors)
	{
		if (IInteractionTarget::Execute_IsInteractionAllowed(LInteraction.GetObject(), LOwner))
		{
			if (bStart)
				IInteractionTarget::Execute_OnInteractionStart(LInteraction.GetObject(), LOwner);
			else
				IInteractionTarget::Execute_OnInteractionStop(LInteraction.GetObject(), LOwner);
		}
	}
}

TScriptInterface<IInteractionTarget> UInteractionBaseComponent::GetFirstInteractionActor() const
{
	if (CurrentInteractionActors.IsValidIndex(0))
	{
		return CurrentInteractionActors[0];
	}

	return nullptr;
}

void UInteractionBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsActive())
	{
		const auto LCurrentFocused = GetInteractionFocusedActors();
		const auto LOldInteractionActors = CurrentInteractionActors;
	
		for (auto& LInteraction : LOldInteractionActors)
		{
			if (IsValid(LInteraction.GetObject()))
			{
				if (!LCurrentFocused.Contains(LInteraction.GetObject()))
				{
					IInteractionTarget::Execute_OnInteractionFocusLost(LInteraction.GetObject(), GetOwner<APawn>());
					CurrentInteractionActors.Remove(LInteraction);
				}
			}
		}

		for (auto LFocused : LCurrentFocused)
		{
			if (IsValid(LFocused) && LFocused->Implements<UInteractionTarget>())
			{
				if (!LOldInteractionActors.Contains(LFocused))
				{
					IInteractionTarget::Execute_OnInteractionFocusStart(LFocused, GetOwner<APawn>());
					CurrentInteractionActors.Add({ LFocused });
				}
			}
		}
	}
}

void UInteractionBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FindSpringArmComponent();
}

USpringArmComponent* UInteractionBaseComponent::FindSpringArmComponent()
{
	if (!IsValid(SpringArmComponent))
	{
		auto LOwnerAsPawn = GetOwner<APawn>();

		if (IsValid(LOwnerAsPawn))
		{
			SpringArmComponent = LOwnerAsPawn->FindComponentByClass<USpringArmComponent>();
		}
	}

	return SpringArmComponent;
}

float UInteractionBaseComponent::GetFullInteractionDistance() const
{
	return (IsValid(SpringArmComponent) ? InteractionDistance + SpringArmComponent->TargetArmLength : InteractionDistance);
}

void UInteractionBaseComponent::ServerInteract_Implementation(bool bStart)
{
	Interact(bStart);
}

void UInteractionBaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME_CONDITION(UInteractionBaseComponent, CollisionChannel, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInteractionBaseComponent, InteractionDistance, COND_OwnerOnly);
}
