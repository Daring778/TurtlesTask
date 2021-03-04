// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CppTurtle.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "TurtleSpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnEvent);

UCLASS()
class TURTLESTASK_API ATurtleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurtleSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* SceneComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* SpawnerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurtleSpawn", meta = (MakeEditWidget))
		FVector FinishPosition; // target loacation turtle for moving

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurtleSpawn")
		TSubclassOf<ACppTurtle> SpawnClass; // variable turtle class

	UPROPERTY(BlueprintReadOnly)
		ACppTurtle* TurtleRef; // spawned turtle actor reference
		
		void F_Spawn(); // spawn function

	UFUNCTION(BlueprintCallable, Category = "TurtleSpawn")
		void StartSpawn(); // Callable function for start spawning

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurtleSpawn")
		UParticleSystem* SpawnEffect; // Particle effect variable

		FTimerHandle TimerDelaySpawn; // timer delay between spawn effects and spawn turtle

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurtleSpawn")
		FVector SpawnEffectScale; // Particle effect spawn scale

		FTransform EffectSpawnTransform; // // Particle effect

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurtleSpawn")
		float DelaySpawn; // delay between spawn effects and spawn turtle

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "SpawnerAudio")
		UAudioComponent* SpawnerAudioComp; //  Audio Component for start spawn turtle

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerAudio")
		USoundCue* SpawnCueSound; // sound cue variable for SpawnerAudioComp

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerAudio")
		bool bPlaySpawnSound; // for control sound playback

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "Spawn event", Category = "TurtleSpawn"))
	FSpawnEvent TurtleSpawningEvent; // spawning event

};
