// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingManager.h"
#include "Agent.h"
#include "Math/Vector.h"
#define AGENT_COUNT 20

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh) {
	UE_LOG(LogTemp, Warning, TEXT("MANAGER INIT"));

	World = world;
	float incr = (PI * 2.f) / AGENT_COUNT;
	for (int i = 0; i < AGENT_COUNT; i++) {
		if (World != nullptr) {
			FRotator rotation = FRotator();
			FVector location = FVector();
			location.X = FMath::Sin(incr * i) * 350.f;
			location.Z = FMath::Cos(incr * i) * 350.f;

			AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
			agent->Init(mesh, i);
			Agents.Add(agent);
		}
	}

	initialized = true;
}

void UFlockingManager::Flock() {
	UE_LOG(LogTemp, Warning, TEXT("FLOCK"));


	for (int i = 0; i < Agents.Num(); i++) {
		AAgent* agent = Agents[i];
		//Alignment(i);
		//agent->Velocity += Alignment(i) + Seperation(i);
		agent->Velocity += Alignment(i) + Cohesion(i) + Seperation(i);// +TendecyToPlace(i);
		LimitVelocity(i);
	}



}




// Alignment -  Make all Agents fly to the center of all nearby Agents
FVector UFlockingManager::Alignment(int id) {

	FVector AveragePos = FVector();
	for (int i = 0; i < Agents.Num(); i++) {
		if (Agents[i]->ID != id) {
			AveragePos += Agents[i]->GetActorLocation();
		}
	}


	AveragePos = AveragePos / Agents.Num() - 1;
	AveragePos = (AveragePos - Agents[id]->GetActorLocation()) / 100;

	UE_LOG(LogTemp, Warning, TEXT("Alignment of %d : %f %f %f"), id, AveragePos.X,AveragePos.Y, AveragePos.Z);
	return AveragePos;
}


// Cohesion - Match Velocity with nearby agents
FVector UFlockingManager::Cohesion(int id) {

	FVector AverageVel = FVector();
	for (int i = 0; i < Agents.Num(); i++) {
		if (Agents[i]->ID != id) {
			AverageVel = AverageVel + Agents[i]->Velocity;
		}
	}
	AverageVel = AverageVel / Agents.Num() - 1;

	AverageVel = (AverageVel - Agents[id]->Velocity) / 8;
	UE_LOG(LogTemp, Warning, TEXT("Cohesion of %d : %f %f %f"), id, AverageVel.X, AverageVel.Y, AverageVel.Z);

	return AverageVel;
}


// Seperation - Make Sure Agents dont over crowd
FVector UFlockingManager::Seperation(int id) {
	FVector Sep = FVector();

	for (int i = 0; i < Agents.Num(); i++) {
		if (Agents[i]->ID != id) {
			if (FVector::Dist(Agents[i]->GetActorLocation(), Agents[id]->GetActorLocation()) < 100.0)
				Sep = Sep - (Agents[i]->GetActorLocation() - Agents[id]->GetActorLocation());

		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Seperation of %d : %f %f %f"), id, Sep.X, Sep.Y, Sep.Z);

	return Sep;
}

FVector UFlockingManager::TendecyToPlace(int id) {
	FVector Place = FVector();
	Place.Y = 100.0;
	Place.Z = 100.0;

	return (Place - Agents[id]->GetActorLocation()) / 100;
}

void UFlockingManager::LimitVelocity(int id) {
	int VelocityLimit = 20.0;

	if (Agents[id]->Velocity.Size() > VelocityLimit) {
		Agents[id]->Velocity = (Agents[id]->Velocity / Agents[id]->Velocity.Size()) * VelocityLimit;
	}
}

