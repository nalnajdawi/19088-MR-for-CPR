using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Collections;

public class Activator : MonoBehaviour
{

    public KeyCode key;
    bool active = false;
    GameObject note;
    public GameObject newBeat;
    
    // Start is called before the first frame update
    void Start()
    {
        active = false;
        note = null;
        InvokeRepeating("SpawnNewBeat", 0.599998f, 0.599998f);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(key) && active)
        {
            Destroy(note);
        }
    }

    void SpawnNewBeat()
    {
        GameObject go = Instantiate(newBeat, new Vector3(0.923f, 0.609f, -.007f), Quaternion.identity) as GameObject;
        go.transform.SetParent(GameObject.Find("WorldHeadLockCanvas").GetComponent<Transform>());
        go.transform.localPosition = new Vector3(0.923f, 0.609f, -0.008f);
    }

    void OnTriggerEnter2D(Collider2D col)
    {
        active = true;
        note = col.gameObject;
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        active = false;
    }
}
