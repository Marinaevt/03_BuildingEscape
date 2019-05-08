// No Copyright

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
private:
	float Reach = 100.f; //in cm

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
	//Ray-cast and grab whats in reach
	void Grab();
	//Called when Grab is released
	void Release();
	//Look for attached physics handle
	void FindPhysicsHandleComponent();
	//Look for and sets up attached input component
	void SetupInputComponent();
	//return hit for first physics body in reach
	FHitResult GetFirstPhysicsBodyInReach() const;
};
