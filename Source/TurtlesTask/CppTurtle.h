// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "CppTurtle.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFinishEvent);

UENUM()
enum TypeMoving // different moving types
{
	SimpeMoving			UMETA(DisplayName = "SimpleMoving"),
	MoveWithPause		UMETA(DisplayName = "MoveWithPause"),
	MoveComplex			UMETA(DisplayName = "MoveComplex"),
};

UCLASS()
class TURTLESTASK_API ACppTurtle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACppTurtle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

		USceneComponent* SceneComp;

		FVector CollisionScale; // for collision size

		void MoveForward(); // Moving Functions
		void MoveBackward();
		void StartMoving();
		void WaitMove();

		void StartRace();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turtle")
		UBoxComponent* TurtleCollision;  // turtle collision

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turtle")
		UStaticMeshComponent* TurtleMesh;	 // turtle mesh

		FVector CurrentPosition; // Current|End location for moving
		FVector EndPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingDelay")
		float DelayStop; // Times for moving
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingDelay")
		float DelayForwardMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingDelay")
		float DelayBackwardMove;

		bool bMoveForward;
		bool bWaitToMove;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turtle")
		float Speed;  // movement speed

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turtle")
		bool bFinish; // for actor finishing

		FTimerHandle TimerMoving; // Move state timers
		FTimerHandle TimerStopMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turtle")
		TEnumAsByte<TypeMoving> MoveType;  // enum variable MoveType

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurtleEffects")
		UParticleSystem* FinishEffect;  // Particle effect variable

	FTransform FinishEffectTransform; // Particle effect spawn transform

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurtleEffects") 
		FVector FinishEffectScale; // Particle effect spawn scale
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TurtleAudio")
		UAudioComponent* FinishAudioComp; // Audio component for finish sounds

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurtleAudio")
		USoundCue* FinishCueSound; // sound cue variable for FinishComp

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurtleAudio")
		bool bPlayFinishSound; // for control sound playback

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, Category = "TurtleSpawn")
		FFinishEvent TurtleFinishEvent; // finishing event

};
