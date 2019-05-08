// No Copyright


#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {

		UE_LOG(LogTemp, Warning, TEXT("%s is here with physics handle"), *(GetOwner()->GetName()));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle controller"), *(GetOwner()->GetName()));
	}
}
void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("%s is here with input controller"), *(GetOwner()->GetName()));

		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing input controller"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	//Try and reach any actors with physics body collision channel set
	GetFirstPhysicsBodyInReach();
}
void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;
	//Getting the player's viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);


	
	///UE_LOG(LogTemp, Warning, TEXT("Loc: %s, Rot: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotator.ToString());
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector()*Reach;
	/*
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);*/

	//Setup query params. False means simple mesh
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	//Line trace (ray-cast) to reach distance
	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	AActor* ActorHit = LineTraceHit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *(ActorHit->GetName()));
	}
	return FHitResult();
}
