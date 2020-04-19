// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGame/Public/FPSCharacter.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
 	//instantiate components
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	//setup hierarchy (Mesh component will be the root component)
	RootComponent = MeshComponent;
	SphereComponent->SetupAttachment(MeshComponent);

	//Collision setup (disabled collisions for mesh components and only activate collision overlap queries for the sphere component against pawns, e.g. player)
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();	
}

void AFPSObjectiveActor::PlayEffects()
{
	//spawn a particle system where the objective is placed
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//try to cast to see if the other actor is indeed an AFPSCharacter (the player)
	AFPSCharacter *character = Cast<AFPSCharacter>(OtherActor);

	if (character) //if the cast succeded we say that is carrying the objective, we spawn the particle system and destroy the objective
	{
		character->bIsCarryingObjective = true;
		PlayEffects();
		Destroy();
	}
}

