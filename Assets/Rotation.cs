using UnityEngine;

public class ChangeEulerAngles : MonoBehaviour
{
    // Set the desired Euler angles
    public Vector3 targetRotation = new Vector3(0f, 90f, 0f);
    int i=0;

    void Update()
    {
        if(i<9000)
        {    
        targetRotation.z = i/100; //replace these angles with the data received from the transmitter
        targetRotation.x = -90f;
        targetRotation.y = 0;
        // Set the Euler angles of the object's rotation directly
        transform.eulerAngles = targetRotation;
        
        ;

        i+=1;
        }
    }
}
