// Fill out your copyright notice in the Description page of Project Settings.


#include "TurtleSpawner.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATurtleSpawner::ATurtleSpawner()
	:
	DelaySpawn(0.5f),
	SpawnEffectScale(0.5f),
	bPlaySpawnSound(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneC"));
	SetRootComponent(SceneComp);

	SpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawner_Mesh"));
	SpawnerMesh->SetupAttachment(SceneComp);

	SpawnerAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Spawner_AudioComp"));
	SpawnerAudioComp->SetupAttachment(SceneComp);
	SpawnerAudioComp->bAutoActivate = false;

}

// Called when the game starts or when spawned
void ATurtleSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnCueSound->IsValidLowLevelFast())
		SpawnerAudioComp->SetSound(SpawnCueSound); // add cue in audio component
	
}

// Called every frame
void ATurtleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurtleSpawner::F_Spawn()
{		
	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = this;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // spawn collision type

	TurtleRef = GetWorld()->SpawnActor<ACppTurtle>(SpawnClass, GetActorLocation(), FRotator(0.f), SpawnParam); // spawning target actor in spawner location
	UE_LOG(LogTemp, Log, TEXT("Spawn!"));

	TurtleSpawningEvent.Broadcast(); // call dispatcher spawn event

	if (TurtleRef->IsValidLowLevelFast())
		TurtleRef->EndPosition = FinishPosition + GetActorLocation(); // set target position (world) in turtle 
	else
	UE_LOG(LogTemp, Log, TEXT("Error Spawn!"));
	

}

void ATurtleSpawner::StartSpawn()
{
	if (TurtleRef == NULL) // spawn if not yet spawn actor on scene
	{
		if (SpawnEffect != NULL)
		{
			if (bPlaySpawnSound)
			{
				SpawnerAudioComp->Play(); // play sounds
			}

			EffectSpawnTransform.SetLocation(GetActorLocation());
			EffectSpawnTransform.SetScale3D(SpawnEffectScale);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, EffectSpawnTransform, true, EPSCPoolMethod::None, true); // spawn particles

		}

		GetWorld()->GetTimerManager().SetTimer(TimerDelaySpawn, this, &ATurtleSpawner::F_Spawn, DelaySpawn, false); // start timer delay turtle spawning
	}
	else
		UE_LOG(LogTemp, Log, TEXT("Exist SpawnActor "));
}

