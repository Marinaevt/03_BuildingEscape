// No Copyright

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	LastDoorOpenTime = 0.f;
	StartingRot = Owner->GetActorRotation();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Pressure plate pointer in %s is null"), *GetOwner()->GetName());
	}
}

void UOpenDoor::OpenDoor()
{
	//Owner->SetActorRotation(StartingRot + FRotator(0.f, fOpenAngle, 0.f));
	OnOpenRequest.Broadcast();
}
void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(StartingRot);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		OpenDoor(); 
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if (GetWorld()->GetTimeSeconds() > LastDoorOpenTime + DoorCloseDelay) {
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (auto& Actor : OverlappingActors) {
		UE_LOG(LogTemp, Warning, TEXT("%s on plate"), *Actor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%f is trigger %f is standing"), TriggerMass, Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass());
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}