// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FlockingManager.generated.h"

/**
 * 
 */
UCLASS()
class FLOCKINGGAME_API UFlockingManager : public UObject
{
	public:
		GENERATED_BODY()

		void Init(UWorld* world, UStaticMeshComponent* mesh);
		void Flock();
		FVector Seperation(int id);
		FVector Cohesion(int id);
		FVector Alignment(int id);
		FVector TendecyToPlace(int id);
		void LimitVelocity(int id);

	private:
		UWorld* World;
		bool initialized;
		TArray<class AAgent*> Agents;
		FTimerHandle FlockingTimerhandle;
		//FVector AverageVel;
};
