// Copyright Sean Barton 2020

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
    // Checking for the Physic handle component
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if(PhysicsHandle) {
        // The physics handle is found
    } else {
        UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle present on %s"), *GetOwner()->GetName())
    }

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the player's viewpoint
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );
    
    // Draw a line from player showing the reach.
    
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    DrawDebugLine(
        GetWorld(),
        PlayerViewPointLocation,
        LineTraceEnd,
        FColor(0, 255, 0),
        false,
        0.f,
        0,
        5.f
                  );
    
    FHitResult Hit;
    // Ray-cast out to a certain distance (Reach)
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
    
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT Hit,
                                            PlayerViewPointLocation,
                                            LineTraceEnd,
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParams
                                            );
    
    // Logging out to test
    AActor* ActorHit = Hit.GetActor();
    
    if (ActorHit) {
        UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()))
    }
    
    // See what it hits
}

