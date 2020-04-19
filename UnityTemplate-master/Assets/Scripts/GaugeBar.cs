using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GaugeBar : MonoBehaviour
{
    private Image barImage;
    private Depth depth;

    private void Awake()
    {
        barImage = transform.Find("Bar").GetComponent<Image>();

        depth = new Depth();
    }

    private void Update()
    {
        depth.Update();
        float depthPercentage = depth.GetDepthNormalized();
        if (depthPercentage >= 0.57f && depthPercentage <= 0.69f)
        {
            barImage.color = Color.green;
        } else
        {
            barImage.color = Color.red;
        }
        barImage.fillAmount = depth.GetDepthNormalized();
    }
}

public class Depth
{
    public const float DEPTH_MIN = 2f;
    public const float DEPTH_MAX = 2.4f;
    public const float BAR_MAX = 3.5f;

    private float depth;
    private float decayRate;

    public Depth()
    {
        depth = 0f;
        decayRate = 0.6f;
    }

    public void Update()
    {
        if (Input.GetKeyDown(KeyCode.A))
        {
            depth = 1.5f;
        }
        if (Input.GetKeyDown(KeyCode.S))
        {
            depth = 2.2f;
        }
        if (Input.GetKeyDown(KeyCode.D))
        {
            depth = 3f;
        }
        depth -= decayRate * Time.deltaTime;
        depth = Mathf.Clamp(depth, 0f, BAR_MAX);
    }

    public void SetDepth(float amt)
    {
        depth = amt;
    }

    public float GetDepthNormalized()
    {
        return depth / BAR_MAX;
    }
}
