// Copyright Serinc All Rights Reserved.

#include "InteractionSingleComponent.h"
#include "GameFramework/SpringArmComponent.h"

UInteractionSingleComponent::UInteractionSingleComponent()
{

}

TArray<AActor*> UInteractionSingleComponent::GetInteractionFocusedActors_Implementation() const
{
	AActor* LAnswer = nullptr;

	auto LOwnerAsPawn = GetOwner<APawn>();
	if (IsValid(LOwnerAsPawn))
	{
		auto LPlayerController = LOwnerAsPawn->GetController<APlayerController>();
		if (IsValid(LPlayerController) && IsValid(LPlayerController->PlayerCameraManager))
		{			
			const auto LCameraManager = LPlayerController->PlayerCameraManager;
			
			FHitResult LHitResult;
	
			FVector LViewLocation;
			FRotator LViewRotation;

			LCameraManager->GetCameraViewPoint(LViewLocation, LViewRotation);
		
			const FVector LocationStart = LViewLocation;
			const FVector LocationEnd = LocationStart + LViewRotation.Vector() * GetFullInteractionDistance();		

			static FName TraceTag = TEXT("InteractionSingle");
		
			FCollisionQueryParams CollisionQueryParams(TraceTag, false, LOwnerAsPawn);
				
			if (GetWorld()->LineTraceSingleByChannel(LHitResult, LocationStart, LocationEnd, CollisionChannel, CollisionQueryParams))
			{
				LAnswer = LHitResult.GetActor();
			}
		}
	}

	return { LAnswer };
}


